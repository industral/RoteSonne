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

#include "ArtistList_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace PlayList {

            // Singleton
            ArtistList_UI * ArtistList_UI::_artistListUI = NULL;

            ArtistList_UI * ArtistList_UI::Instance() {
              if (_artistListUI == NULL) {
                _artistListUI = new ArtistList_UI();
              }
              return _artistListUI;
            }

            // --------------------------------------------------------------------
            // Public methods
            // --------------------------------------------------------------------

            ArtistList_UI::~ArtistList_UI() {
            }

            void ArtistList_UI::init(QWidget *widget) {
              this -> trackList = TrackList::TrackList_UI::Instance();
              this -> albumList = AlbumList_UI::Instance();

              this -> widget = widget;
              this -> findChilds();
              this -> addHandlers();

              // connect to existing connection
              this -> db = QSqlDatabase::database();
            }

            void ArtistList_UI::setPlayList() {
              // select all available artist
              this -> query = this -> db.exec("SELECT DISTINCT artist FROM collection");

              // add first item as "All Artists"
              this -> artistListComponent -> addItem("All Artists");

              // populate list
              while (query.next()) {
                QString artist = query.value(0).toString();

                if (artist.isEmpty()) {
                  artist = "Unknown Artist";
                }
                this -> artistListComponent -> addItem(artist);
              }
            }

            void ArtistList_UI::dropPlayList() {
              this -> artistListComponent -> clear();
            }

            QString ArtistList_UI::getCurrentArtist() const {
              return this -> currentArtist;
            }

            void ArtistList_UI::setCurrentArtist(const QString &artist) {
              this -> currentArtist = artist;
            }

            void ArtistList_UI::selectArtist(const QString & artist) {
              // find appropriate element by artist name
              QList<QListWidgetItem *> list = this -> artistListComponent -> findItems(artist, Qt::MatchExactly);

              // get element
              QListWidgetItem * widgetItem = list.at(0);

              // emit signal (this will change TrckList widget view)
              setFilter(widgetItem, widgetItem);

              // set focus explicitly on ArtistList widget
              widgetItem -> setSelected(true);

              // scroll to view
              this -> artistListComponent -> scrollToItem(widgetItem, QAbstractItemView::PositionAtCenter);
            }

            // --------------------------------------------------------------------
            // Private methods
            // --------------------------------------------------------------------

            ArtistList_UI::ArtistList_UI() {
            }

            void ArtistList_UI::findChilds() {
              // track list
              this -> artistListComponent = this -> widget -> findChild<QListWidget *> ("artistList");
            }

            void ArtistList_UI::addHandlers() {
              connect(this -> artistListComponent, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem * )),
                  this, SLOT(setFilter(QListWidgetItem *, QListWidgetItem * )));

              connect(this -> artistListComponent, SIGNAL(itemDoubleClicked(QListWidgetItem * )), this,
                  SLOT(play(QListWidgetItem *)));
            }

            // --------------------------------------------------------------------
            // Private slots
            // --------------------------------------------------------------------

            void ArtistList_UI::setFilter(QListWidgetItem * current, QListWidgetItem * previous) {
              if (current != NULL && previous != NULL) {
                QString artist = current -> text();
                QString filter = ""; // default filter value

                if (!artist.compare("Unknown Artist")) {
                  artist = "";
                }

                // update current artist variable
                this -> setCurrentArtist(artist);

                // if it not All Artists
                if (artist.compare("All Artists")) {
                  filter = "artist=\"" + artist + "\" ORDER BY album, tracknum";
                }

                // drop track list
                this -> trackList -> dropPlayList();

                // drop album list
                this -> albumList -> dropPlayList();

                // set filter
                this -> trackList -> setFilter(filter);

                // set track list
                this -> trackList -> setPlayList();

                // set album list
                this -> albumList -> setPlayList();
              }
            }

            void ArtistList_UI::play(QListWidgetItem * item) {
              this -> albumList -> playFirstItem();
            }

          }
        }
      }
    }
  }
}
