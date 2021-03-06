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

#ifndef _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_PLAYLIST_ALBUMLIST_HPP_
#define _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_PLAYLIST_ALBUMLIST_HPP_

#include <include.hpp>

#include <QListWidget>
#include <QListWidgetItem>

#include "AbstractPlayList.hpp"
#include "TrackList/TrackList_UI.hpp"
#include "ArtistList_UI.hpp"
#include "../Cover_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace PlayList {

            class ArtistList_UI;

            namespace TrackList {
              class TrackList_UI;
            }

            class AlbumList_UI: public QWidget, virtual public AbstractPlayList {
              Q_OBJECT
              public:
                static AlbumList_UI * Instance();

                /**
                 * Default destructor.
                 */
                ~AlbumList_UI();

                void init(QWidget *widget);
                void setPlayList();
                void dropPlayList();
                void playFirstItem();
              private:
                static AlbumList_UI * _albumListUI;

                /**
                 * Default constructor.
                 */
                AlbumList_UI();

                QWidget * widget;

                QSqlDatabase db;
                QSqlQuery query;

                QListWidget * albumListComponent;
                ::RoteSonne::UI::Widgets::MainWindow::Widgets::Cover_UI * cover;

                TrackList::TrackList_UI * trackList;
                ArtistList_UI * artistList;

                /**
                 * Set album cover when user click.
                 * @param[in] artist Artist
                 * @param[in] album Album
                 */
                void setCover(const QString &artist, const QString &album = "");
                void findChilds();
                void addHandlers();

              private slots:
                void setFilter(QListWidgetItem * current, QListWidgetItem * previous);
                void play(QListWidgetItem * item);
            };
          }
        }
      }
    }
  }
}

#endif
