/*******************************************************************************
 * Copyright (c) 2009-2010, Alex Ivasyuv                                       *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 * 1. Redistributions of source code must retain the above copyright           *
 *    notice, this list of conditions and the following disclaimer.            *
 * 2. Redistributions in binary form must reproduce the above copyright        *
 *    notice, this list of conditions and the following disclaimer in the      *
 *    documentation and/or other materials provided with the distribution.     *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY Alex Ivasyuv ''AS IS'' AND ANY                 *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL Alex Ivasyuv BE LIABLE FOR ANY DIRECT,        *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES          *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;*
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF    *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           *
 ******************************************************************************/

#include "AlbumList_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace PlayList {

            // Singleton
            AlbumList_UI * AlbumList_UI::_albumListUI = NULL;

            AlbumList_UI * AlbumList_UI::Instance() {
              if (_albumListUI == NULL) {
                _albumListUI = new AlbumList_UI();
              }
              return _albumListUI;
            }

            // --------------------------------------------------------------------
            // Public methods
            // --------------------------------------------------------------------

            AlbumList_UI::~AlbumList_UI() {
            }

            void AlbumList_UI::init(QWidget *widget) {
              this -> trackList = TrackList::TrackList_UI::Instance();
              this -> artistList = ArtistList_UI::Instance();
              this -> cover = Cover_UI::Instance();

              this -> widget = widget;
              this -> findChilds();
              this -> addHandlers();

              // connect to existing connection
              this -> db = QSqlDatabase::database();
            }

            void AlbumList_UI::setPlayList() {
              QString currentArtist = this -> artistList -> getCurrentArtist();
              QString query = "SELECT album FROM collection";
              QString whereClause = QString(" WHERE artist=\"%1\"").arg(currentArtist);

              if (!currentArtist.isEmpty()) {
                query += whereClause;
              }

              query += " GROUP BY album";

              // select all available artist
              QSqlQuery q(this -> db);

              q.exec(query);

              /*
               * sqlite doesn't support return size of affected row. So we should count it manually.
               */
              int length = 0;

              // populate list
              while (q.next()) {
                QString album = q.value(0).toString();

                if (album.isEmpty()) {
                  album = "Unknown Album";
                }
                this -> albumListComponent -> addItem(album);
                ++length;
              }

              if (length == 1) {
                this -> setCover(currentArtist);
              }

            }

            void AlbumList_UI::dropPlayList() {
              this -> albumListComponent -> clear();
            }

            void AlbumList_UI::playFirstItem() {
              // select first element
              QListWidgetItem * item = this -> albumListComponent -> item(0);

              // apply filter
              setFilter(item, item);

              // select item explicitly
              item -> setSelected(true);

              // play
              play(item);
            }

            // --------------------------------------------------------------------
            // Private methods
            // --------------------------------------------------------------------

            AlbumList_UI::AlbumList_UI() {
            }

            void AlbumList_UI::findChilds() {
              // track list
              this -> albumListComponent = this -> widget -> findChild<QListWidget *> ("albumList");
            }

            void AlbumList_UI::addHandlers() {
              connect(this -> albumListComponent, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem * )),
                  this, SLOT(setFilter(QListWidgetItem *, QListWidgetItem * )));

              connect(this -> albumListComponent, SIGNAL(itemDoubleClicked( QListWidgetItem * )), this,
                  SLOT(play( QListWidgetItem * )));
            }

            void AlbumList_UI::setCover(const QString &artist, const QString &album) {
              //TODO: rewrite query
              QString query;

              if (!album.size()) {
                query = QString("SELECT fileName FROM collection WHERE artist=\"%1\" LIMIT 0,1").arg(artist);
              } else {
                query = QString("SELECT fileName FROM collection WHERE artist=\"%1\" and album=\"%2\" LIMIT 0,1").arg(
                    artist).arg(album);
              }

              // select all available artist
              QSqlQuery q(this -> db);

              q.exec(query);
              QString track;

              while (q.next()) {
                track = q.value(0).toString();
              }

              this -> cover -> setCover(track.toStdString().c_str());
            }

            // --------------------------------------------------------------------
            // Private slots
            // --------------------------------------------------------------------

            void AlbumList_UI::setFilter(QListWidgetItem * current, QListWidgetItem * previous) {
              // if focus changes to another window, item will be null
              if (current != NULL) {
                QString album = current -> text();

                QString artist = this -> artistList -> getCurrentArtist();

                QString albumFilter = ""; // default filter value
                QString artistFilter = "";

                if (!album.compare("Unknown Album")) {
                  album = "";
                }

                artistFilter = QString("artist=\"%1\"").arg(artist);
                albumFilter = QString(" album=\"%1\" ORDER BY tracknum").arg(album);

                // set cover
                this -> setCover(artist, album);

                // drop track list
                this -> trackList -> dropPlayList();

                // set filter
                this -> trackList -> setFilter(artistFilter + " and " + albumFilter);

                // set track list
                this -> trackList -> setPlayList();
              }
            }

            void AlbumList_UI::play(QListWidgetItem * item) {
              /* first check if any artist selected in ArtistList widget */
              QString selectedArtist = this -> artistList -> getCurrentArtist();

              // if Artist is not selected, find it by selected Album
              if (selectedArtist.isEmpty()) {
                QString selectedAlbum = item -> text();
                QString query = QString("SELECT artist FROM collection WHERE album = '%1' GROUP BY artist").arg(
                    selectedAlbum);

                QSqlQuery q(this -> db);
                q.exec(query);

                int count = 0;

                QString foundArtist = "";
                while (q.next()) {
                  ++count;
                  foundArtist = q.value(0).toString();
                }

                if (count == 1) {
                  this -> artistList -> selectArtist(foundArtist);
                }

                // find item by selected album
                QList<QListWidgetItem *> list =
                    this -> albumListComponent -> findItems(selectedAlbum, Qt::MatchExactly);

                // get first element
                QListWidgetItem * widgetItem = list.at(0);

                // apply filter
                setFilter(widgetItem, widgetItem);

                // run this method again
                play(widgetItem);

                // select explicitly
                widgetItem -> setSelected(true);
              } else {
                this -> trackList -> playFirstTrack();
              }
            }

          }
        }
      }
    }
  }
}
