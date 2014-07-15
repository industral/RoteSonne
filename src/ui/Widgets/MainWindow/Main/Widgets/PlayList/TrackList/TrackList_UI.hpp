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

#ifndef _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_PLAYLIST_TRACKLIST_HPP_
#define _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_PLAYLIST_TRACKLIST_HPP_

#include <include.hpp>

#include "../AbstractPlayList.hpp"
#include "ItemDelegate.hpp"

// include Collection
#include <Collection.hpp>

// include MainWindow
#include <UI/Widgets/MainWindow/MainWindow_UI.hpp>

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {

        class MainWindow_UI;

        namespace Widgets {
          namespace PlayList {
            namespace TrackList {

              class TrackList_UI: public QObject, virtual public AbstractPlayList {
                Q_OBJECT

                public:
                  static TrackList_UI * Instance();

                  /**
                   * Default destructor.
                   */
                  ~TrackList_UI();

                  void init(QWidget *widget);
                  void playFirstTrack();
                  void setPlayList();
                  void setFilter(const QString &filter);
                  void dropPlayList();
                  QModelIndex & getSelectedIndex();
                  QModelIndex & getPlayedIndex();
                  void refresh(const QModelIndex & index);
                  int getRowCount();
                  void playNextTrack();

                private:
                  static TrackList_UI * _playListUI;

                  /**
                   * Default constructor.
                   */
                  TrackList_UI();

                  QWidget * widget;
                  RoteSonne::UI::Widgets::MainWindow::MainWindow_UI * mainWindow;
                  QSqlDatabase db;
                  QSqlTableModel * model;
                  QTableView * trackListComponent;
                  Configuration * cfg;
                  ::RoteSonne::Collection * collectionDb;

                  QString filter;
                  QModelIndex selectedIndex;
                  QModelIndex playedIndex;

                  void findChilds();
                  bool openDbConnection();
                  void closeDbConnection();
                  void addHandlers();

                private slots:
                  void play(const QModelIndex &index);
                  void setView(const QModelIndex & index);
                  void initLocation(const QModelIndex &index);
                  void showInfo(const QModelIndex &index);
                  void activateEmelents(const QModelIndex &index);
                  void setPlayingIndex(const QModelIndex &index);
                  void refreshView();

                signals:
                  void doubleClicked(const QModelIndex &index);
                  void pressed(const QModelIndex &index);
              };
            }
          }
        }
      }
    }
  }
}

#endif
