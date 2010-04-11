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

#include "MainWindow_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {

        // --------------------------------------------------------------------
        // Public methods
        // --------------------------------------------------------------------

        // Singleton
        MainWindow_UI * MainWindow_UI::_mainWindowUI = NULL;

        MainWindow_UI * MainWindow_UI::Instance() {
          if (_mainWindowUI == NULL) {
            _mainWindowUI = new MainWindow_UI();
          }
          return _mainWindowUI;
        }

        MainWindow_UI::~MainWindow_UI() {
          delete this -> player;
          this -> player = NULL;

          delete this -> trackinfo;
          this -> trackinfo = NULL;

          delete this -> trackList;
          this -> trackList = NULL;

          delete this -> cover;
          this -> cover = NULL;

          delete this -> library;
          this -> library = NULL;

          delete this -> albumList;
          this -> albumList = NULL;

          delete this -> artistList;
          this -> artistList = NULL;
        }

        QWidget * MainWindow_UI::getUI() const {
          return this -> widget;
        }

        void MainWindow_UI::init() {
          this -> sliderPosition = 0;
          this -> timer = new QTimer(this);

          this -> player = new Player();

          const QString soundDriver = "AO";
          const QString soundDevice = "";

          if (this -> player -> setAudioDriver(soundDriver.toStdString(), soundDevice.toStdString()) == false) {
            const QString message = QString("Failed to initialized Audio System with %1 driver").arg(soundDriver);
            Window::getPopup("Failed to initialize audio system", message);
            exit(1);
          }

          // in start player is in STOP status
          this -> player -> setPlayerStatus(Player::Stop);

          // load UI resource file.
          this -> widget = LoadUI::loadUI(":/forms/designer/Main.ui");

          /* load widgets */

          // library
          this -> library = new Widgets::Library::Library_UI(this -> widget);

          // artist list
          this -> artistList = Widgets::PlayList::ArtistList_UI::Instance();
          this -> artistList -> init(this -> widget);

          // album list
          this -> albumList = Widgets::PlayList::AlbumList_UI::Instance();
          this -> albumList -> init(this -> widget);

          // track list
          this -> trackList = Widgets::PlayList::TrackList::TrackList_UI::Instance();
          this -> trackList -> init(this -> widget);

          // cover
          this -> cover = Widgets::Cover_UI::Instance();
          this -> cover -> init(this -> widget);

          // track info
          this -> trackinfo = new Widgets::TrackInfo_UI::TrackInfo_UI(this -> widget);

          // get all components from widget
          this -> findChilds();

          // set playLists
          this -> setPlayLists();

          // attach handles.
          this -> addHandlers();
        }

        void MainWindow_UI::play(const QModelIndex &index) {
          // change play/pause icon
          if (((this -> player -> getPlayerStatus()) == Player::Stop) || ((this -> player -> getPlayerStatus())
              == Player::Pause)) {
            // change icon
            this -> playPauseButton -> setIcon(QIcon(":/images/media-playback-pause.png"));
          }

          if (this -> player -> getPlayerStatus() == Player::Pause) {
            this -> player -> play(this -> playingFileId, true);
            // change Play status
            this -> player -> setPlayerStatus(Player::Play);
          } else {

            // check if previous song is playing
            if (this -> player -> getPlayerStatus() == Player::Play) {
              this -> player -> stop(this -> playingFileId);
            }

            // get file name
            string fileName = index.sibling(index.row(), 1).data().toString().toStdString();

            // generate new fileId
            this -> fileId = SilentMedia::Utils::Random::getRandomId(fileName);

            if (this -> player -> open(fileName, this -> fileId)) {
              // change Play status
              this -> player -> setPlayerStatus(Player::Play);

              // set playing fileId
              this -> playingFileId = this -> fileId;

              // begin play
              this -> player -> play(this -> playingFileId);
              this -> beginUpdateSlider();
            }
          }
        }

        void MainWindow_UI::showInfo(const QModelIndex & index) {
          string fileName = index.sibling(index.row(), 1).data().toString().toStdString();
          this -> fileId = SilentMedia::Utils::Random::getRandomId(fileName);

          // before doing something we should check if it open success
          if (this -> player -> open(fileName, this -> fileId)) {
            // show information
            this -> trackinfo -> showInfo(this -> fileId, this -> player);

            this -> player -> close(this -> fileId);

            // show cover
            this -> cover -> setCover(fileName);
          }
        }

        void MainWindow_UI::activateEmelents(const QModelIndex & index) {
          this -> playPauseButton -> setEnabled(true);
          this -> playerSlider -> setEnabled(true);
        }

        // --------------------------------------------------------------------
        // Private methods
        // --------------------------------------------------------------------

        MainWindow_UI::MainWindow_UI() {
        }

        void MainWindow_UI::findChilds() {
          //TODO: move it
#ifdef OSS
          QWidget * ssss
          = this -> widget -> findChild <QWidget *> ("Mixer");

          RoteSonne::UI::Widgets::Mixer::OSS::Mixer * mixer = new RoteSonne::UI::Widgets::Mixer::OSS::Mixer();
          QWidget * mixerWidget = mixer -> retW();
          QHBoxLayout *layout = new QHBoxLayout;
          layout -> addWidget(mixerWidget);

          ssss ->setLayout(layout);
#endif

          // tack list component
          this -> trackListComponent = this -> widget -> findChild<QTableView *> ("trackList");

          // player slider
          this -> playerSlider = this -> widget -> findChild<QSlider *> ("playerSlider");

          // play/pause button
          this -> playPauseButton = this -> widget -> findChild<QPushButton *> ("playPauseButton");

          // previous track button
          this -> prevButton = this -> widget -> findChild<QPushButton *> ("prevButton");

          // next track button
          this -> nextButton = this -> widget -> findChild<QPushButton *> ("nextButton");
        }

        void MainWindow_UI::addHandlers() {
          // add "About Qt" handler
          connect(this -> widget -> findChild<QAction *> ("actionAbout_Qt"), SIGNAL(triggered()), this, SLOT(aboutQt()));

          // add "About Player" handler
          connect(this -> widget -> findChild<QAction *> ("actionAbout"), SIGNAL(triggered()), this, SLOT(about()));

          // add "Collection" handler
          connect(this -> widget -> findChild<QAction *> ("actionCollection"), SIGNAL(triggered()), this,
              SLOT(collectionPreferences()));

          // add "Preferences" handler
          connect(this -> widget -> findChild<QAction *> ("actionPreferences"), SIGNAL(triggered()), this,
              SLOT(preferences()));

          // add "Play/Pause" handler
          connect(this -> playPauseButton, SIGNAL(clicked()), this, SLOT(playPauseToggle()));

          // SLIDER
          // Update slider position every n seconds
          connect(this -> timer, SIGNAL(timeout()), this, SLOT(updateSliderPosition()));

          // write slider value
          connect(this -> playerSlider, SIGNAL (valueChanged (int)), this, SLOT (writeSliderPosition (int)));
          // while move slider, disconnect slider
          connect(this -> playerSlider, SIGNAL(sliderPressed()), this, SLOT(stopUpdateSlider()));
          // when slider released, set song on seek position
          connect(this -> playerSlider, SIGNAL(sliderReleased()), this, SLOT(setSeek()));
          connect(this -> playerSlider, SIGNAL(sliderReleased()), this, SLOT(beginUpdateSlider()));
        }

        void MainWindow_UI::setPlayLists() {
          // setup play lists
          this -> artistList -> setPlayList();
          this -> albumList -> setPlayList();
          this -> trackList -> setPlayList();
        }

        void MainWindow_UI::dropPlayList() {
          this -> trackList -> dropPlayList();
        }

        // --------------------------------------------------------------------
        // Slots
        // --------------------------------------------------------------------

        void MainWindow_UI::aboutQt() {
          QApplication::aboutQt();
        }

        void MainWindow_UI::about() {
          //TODO: move to const
          //TODO: make checks for all widgets!
          QWidget * widget = LoadUI::loadUI(":/forms/designer/About.ui");
          if (widget != NULL) {
            widget -> show();
          } else {
            cerr << "Failed to open widget" << endl;
          }
        }

        void MainWindow_UI::collectionPreferences() {
          RoteSonne::UI::Widgets::Collection::Collection_UI * collection =
              new RoteSonne::UI::Widgets::Collection::Collection_UI();
          collection -> show();
        }

        void MainWindow_UI::preferences() {
          RoteSonne::UI::Widgets::Preferences::Preferences_UI * preferences =
              new RoteSonne::UI::Widgets::Preferences::Preferences_UI();
          preferences -> show();
        }

        void MainWindow_UI::pause(const QModelIndex &index) {
          //      string fileName = index.sibling (index.row(), 1).data().toString().toStdString();
          this -> player -> pause(this -> playingFileId);
          // change play status
          this -> player -> setPlayerStatus(Player::Pause);
        }

        void MainWindow_UI::playPauseToggle() {
          // toggle status play/pause button
          /*
           * In first time when we start player, play/pause button is disabled.
           * We should select a song from list to activate play/pause button.
           * When we click on play/pause button, it toggle play status and change
           * icon to pause. After clicking one more time, it again toggle play
           * status and change icon to play.
           */
          if (((this -> player -> getPlayerStatus()) == Player::Stop) || ((this -> player -> getPlayerStatus())
              == Player::Pause)) {
            // change icon
            this -> playPauseButton -> setIcon(QIcon(":/images/media-playback-pause.png"));
            // play
            this -> play(this -> trackList -> getIndex());
          } else if ((this -> player -> getPlayerStatus()) == Player::Play) {
            // change icon
            this -> playPauseButton -> setIcon(QIcon(":/images/media-playback-start.png"));
            // pause
            this -> pause(this -> trackList -> getIndex());
          }
        }

        void MainWindow_UI::updateSliderPosition() {
          // write slider position
          this -> sliderPosition = this -> player -> getSeek(this -> playingFileId);

          // set slider position
          this -> setSliderPosition();
        }

        void MainWindow_UI::writeSliderPosition(int position) {
          this -> sliderPosition = position;
          cout << "Set position to: " << this -> sliderPosition << endl;
        }

        void MainWindow_UI::beginUpdateSlider() {
          this -> timer -> start(100);
        }

        void MainWindow_UI::stopUpdateSlider() {
          this -> timer -> stop();
        }

        void MainWindow_UI::setSliderPosition() {
          this -> playerSlider -> setSliderPosition(this -> sliderPosition);
        }

        void MainWindow_UI::setSeek() {
          this -> player -> setSeek(this -> playingFileId, this -> sliderPosition);
        }

      }
    }
  }
}
