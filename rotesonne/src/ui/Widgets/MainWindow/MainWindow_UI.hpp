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

#ifndef _ROTESONNE_UI_WIDGETS_MAINWINDOW_MAINWINDOW_HPP_
#define _ROTESONNE_UI_WIDGETS_MAINWINDOW_MAINWINDOW_HPP_

#include <include.hpp>

// include UI loader
#include <UI/LoadUI.hpp>

// include Player class
#include <Player.hpp>

// include MainWindow widgets
#include "Main/Widgets/Library/Library_UI.hpp"
#include "Main/Widgets/TrackInfo_UI.hpp"
#include "Main/Widgets/Cover_UI.hpp"
#include "Main/Widgets/PlayList/TrackList/TrackList_UI.hpp"
#include "Main/Widgets/PlayList/ArtistList_UI.hpp"
#include "Main/Widgets/PlayList/AlbumList_UI.hpp"

// include Collection/Preferences widget
#include <UI/Widgets/Collection/Collection_UI.hpp>
#include <UI/Widgets/Preferences/Preferences_UI.hpp>

// include Mixer
#include <libsml/all.hpp>

// include popup window
#include <UI/Window.hpp>

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {

        namespace Widgets {
          namespace PlayList {
            namespace TrackList {
              class TrackList_UI;
            }
          }
        }

        /**
         * Main player class.
         */
        class MainWindow_UI: public QMainWindow {
          Q_OBJECT

          public:
            static MainWindow_UI * Instance();

            /**
             * Default destructor.
             */
            ~MainWindow_UI();

            /**
             * Return widget pointer.
             * @return widget pointer.
             */
            QWidget * getUI() const;

            void init();
            void play(const QModelIndex &index);
            void showInfo(const QModelIndex &index);
            void activateEmelents(const QModelIndex &index);
            void setPlayingStatus(const string fileId, const QString message = "");

          private slots:
            void aboutQt();
            void about();
            void collectionPreferences();
            void preferences();
            void pause(const QModelIndex &index);

            /**
             * Timer function , update slider position.
             */
            void updateSliderPosition();
            void setSeek();

            void writeSliderPosition(int position);

            void beginUpdateSlider();
            void stopUpdateSlider();

            /**
             * Set slider in position from sliderPosition variable.
             */
            void setSliderPosition();

            /**
             * Toggle status SLOT.
             */
            void playPauseToggle();

          private:
            static MainWindow_UI * _mainWindowUI;

            /**
             * Default constructor.
             */
            MainWindow_UI();

            QWidget * widget;
            Player * player;

            // widgets
            Widgets::Library::Library_UI * library;
            Widgets::TrackInfo_UI * trackinfo;
            Widgets::PlayList::TrackList::TrackList_UI * trackList;
            Widgets::PlayList::ArtistList_UI * artistList;
            Widgets::PlayList::AlbumList_UI * albumList;
            Widgets::Cover_UI * cover;

            QPushButton * playPauseButton;
            QPushButton * prevButton;
            QPushButton * nextButton;

            QTableView * trackListComponent;

            QSlider * playerSlider;
            QTimer * timer;

            // selected fileId
//            string fileId;

            // playing current fileId
            string playingFileId;

            // slider current position
            int sliderPosition;

            void findChilds();
            void addHandlers();
            void setPlayLists();
            void dropPlayList();
        };
      }
    }
  }
}

#endif
