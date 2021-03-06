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

#ifndef _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_PLAYLIST_ARTISTLIST_HPP_
#define _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_PLAYLIST_ARTISTLIST_HPP_

#include <include.hpp>

#include "AbstractPlayList.hpp"
#include "TrackList/TrackList_UI.hpp"
#include "AlbumList_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace PlayList {

            class AlbumList_UI;

            namespace TrackList {
              class TrackList_UI;
            }

            class ArtistList_UI: public QWidget, virtual public AbstractPlayList {

              Q_OBJECT

              public:
                static ArtistList_UI * Instance();

                /**
                 * Default destructor.
                 */
                ~ArtistList_UI();

                void init(QWidget *widget);
                void setPlayList();
                void dropPlayList();

                QString getCurrentArtist() const;
                void setCurrentArtist(const QString &artist);

                /**
                 * Set selected artist in ArtistList widget by artist name.
                 *
                 * @param artist artist name.
                 */
                void selectArtist(const QString & artist);

              private:
                static ArtistList_UI * _artistListUI;

                /**
                 * Default constructor.
                 */
                ArtistList_UI();

                QWidget * widget;

                QSqlDatabase db;
                QSqlQuery query;

                QListWidget * artistListComponent;

                TrackList::TrackList_UI * trackList;
                AlbumList_UI * albumList;

                QString currentArtist;

                void findChilds();
                void addHandlers();

              private slots:
                void setFilter(QListWidgetItem * current, QListWidgetItem * previous);
                void play(QListWidgetItem * item);

                /* actions */
                void contextMenuHandler(const QPoint & pos);

                void addToPlayList();
            };
          }
        }
      }
    }
  }
}

#endif
