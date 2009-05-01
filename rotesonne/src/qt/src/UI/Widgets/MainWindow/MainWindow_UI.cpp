/*******************************************************************************
 * Copyright (c) 2009, Alex Ivasyuv                                            *
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

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      MainWindow_UI::MainWindow_UI() {
        this -> sliderPosition = 0;
        this -> timer = new QTimer(this);

        this -> player = new Player();
        this -> player -> setAudioDriver();

        // in start player is in STOP status
        this -> player -> setPlayerStatus(Player::Stop);

        // load UI resource file.
        this -> widget = LoadUI::loadUI(":/forms/ui/mainWindow.ui");

        // load widgets
        this -> trackinfo = new TrackInfo_UI(this -> widget);
        this -> playlist = new PlayList_UI(this -> widget);

        // get all components from widget
        this -> findChilds();

        // set playList
        this -> setPlayList();

        // attach handles.
        this -> addHandlers();
      }

      MainWindow_UI::~MainWindow_UI() {
        delete this -> player;
        this -> player = NULL;

        delete this -> trackinfo;
        this -> trackinfo = NULL;

        delete this -> playlist;
        this -> playlist = NULL;
      }

      QWidget * MainWindow_UI::getUI() const {
        return this -> widget;
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      void MainWindow_UI::findChilds() {
        // play list
        this -> playList = this -> widget -> findChild < QTableView * > (
            "playList");

        // player slider
        this -> playerSlider = this -> widget -> findChild < QSlider * > (
            "playerSlider");

        // play/pause button
        this -> playPauseButton
            = this -> widget -> findChild < QPushButton * > ("playPauseButton");

        // previous track button
        this -> prevButton = this -> widget -> findChild < QPushButton * > (
            "prevButton");

        // next track button
        this -> nextButton = this -> widget -> findChild < QPushButton * > (
            "nextButton");
      }

      string MainWindow_UI::getRandomID(const string &fileName) {
        boost::mt19937 rng;
        rng.seed(static_cast < unsigned > (std::time(0)));
        boost::uniform_int < > distribution(1, LONG_MAX);
        boost::variate_generator < boost::mt19937&, boost::uniform_int < > >
            die(rng, distribution);

        stringstream out;
        out << fileName << die();

        return out.str();
      }

      void MainWindow_UI::addHandlers() {
        // add "About Qt" handler
        connect(this -> widget -> findChild < QAction * > ("actionAbout_Qt"),
            SIGNAL(triggered()), this, SLOT(aboutQt()));

        // add "About" handler
        connect(this -> widget -> findChild < QAction * > ("actionAbout"),
            SIGNAL(triggered()), this, SLOT(about()));

        // add "Collection" handler
        connect(this -> widget -> findChild < QAction * > ("actionCollection"),
            SIGNAL(triggered()), this, SLOT(collectionPreferences()));

        // add "Play/Pause" handler
        connect(this -> playPauseButton, SIGNAL(clicked()), this, SLOT(
            playPauseToggle()));

        // SLIDER
        // Update slider position every n seconds
        connect(this -> timer, SIGNAL(timeout()), this, SLOT(
            updateSliderPosition()));

        // write slider value
        connect ( this -> playerSlider, SIGNAL (valueChanged (int)), this, SLOT (writeSliderPosition (int)));
        // while move slider, disconnect slider
        connect(this -> playerSlider, SIGNAL(sliderPressed()), this, SLOT(
            stopUpdateSlider()));
        // when slider released, set song on seek position
        connect(this -> playerSlider, SIGNAL(sliderReleased()), this, SLOT(
            setSeek()));
        connect(this -> playerSlider, SIGNAL(sliderReleased()), this, SLOT(
            beginUpdateSlider()));
      }

      void MainWindow_UI::setPlayList() {
        // setup play list
        this -> playlist -> setPlayList(this -> playList);

connect      (this -> playList, SIGNAL (doubleClicked (const QModelIndex & )), this,
          SLOT (play(const QModelIndex & )));
      connect ( this -> playList, SIGNAL (pressed(const QModelIndex &)), this, SLOT (initLocation(const QModelIndex &)));
      connect ( this -> playList, SIGNAL (pressed(const QModelIndex &)), this, SLOT (showInfo(const QModelIndex &)));
      connect ( this -> playList, SIGNAL (pressed(const QModelIndex &)), this, SLOT (acivatePlayPauseButton(const QModelIndex &)));
    }

    // --------------------------------------------------------------------
    // Slots
    // --------------------------------------------------------------------

    void MainWindow_UI::aboutQt() {
      QApplication::aboutQt();
    }

    void MainWindow_UI::about() {
      QWidget *widget = LoadUI::loadUI(":/forms/ui/aboutRoteSonne.ui");
      widget -> show();
    }

    void MainWindow_UI::collectionPreferences() {
      Collection_UI *collection = new Collection_UI(collection);
      collection -> show();
    }

    void MainWindow_UI::play(const QModelIndex &index) {
      // change play/pause icon
      if (((this -> player -> getPlayerStatus()) == Player::Stop) ||
          ((this -> player -> getPlayerStatus()) == Player::Pause)) {
        // change icon
        this -> playPauseButton -> setIcon (QIcon(":/images/images/player_pause.png"));
      }

      if (this -> player -> getPlayerStatus() == Player::Pause) {
        this -> player -> play(this -> playingFileId, true);
        //        this -> beginUpdateSlider();
      } else {

        // check if previous song is playing
        if (this -> player -> getPlayerStatus() == Player::Play) {
          this -> player -> stop(this -> playingFileId);
        }

        // get file name
        string fileName = index.sibling (index.row(), 1).data().toString().toStdString();

        // generate new fileId
        this -> fileId = this -> getRandomID(fileName);

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
      if (((this -> player -> getPlayerStatus()) == Player::Stop) ||
          ((this -> player -> getPlayerStatus()) == Player::Pause)) {
        // change icon
        this -> playPauseButton -> setIcon (QIcon(":/images/images/player_pause.png"));
        // play
        this -> play(this -> index);
      } else if ((this -> player -> getPlayerStatus()) == Player::Play) {
        // change icon
        this -> playPauseButton -> setIcon (QIcon(":/images/images/player_play.png"));
        // pause
        this -> pause(this -> index);
      }
    }

    void MainWindow_UI::showInfo(const QModelIndex & index) {
      string fileName = index.sibling (index.row(), 1).data().toString().toStdString();
      this -> fileId = this -> getRandomID(fileName);

      // before doing something we should check if it open success
      if (this -> player -> open(fileName, this -> fileId)) {
        // show information
        this -> trackinfo -> showInfo(this -> fileId, this -> player);

        this -> player -> close(this -> fileId);
      }
    }

    void MainWindow_UI::initLocation(const QModelIndex & index) {
      this -> index = index;
    }

    void MainWindow_UI::acivatePlayPauseButton(const QModelIndex & index) {
      this -> playPauseButton -> setEnabled(true);
      //      disconnect ( this -> playList, SIGNAL (pressed(const QModelIndex &)), this, SLOT (acivatePlayPauseButton(const QModelIndex &)));
    }

    void MainWindow_UI::updateSliderPosition() {
      // write slider position
      this -> sliderPosition = this -> player -> getSeek (this -> playingFileId);

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
