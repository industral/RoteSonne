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
        this -> player = new Player();
        this -> player -> setAudioDriver("alsa");

        // load UI resource file.
        this -> widget = LoadUI::loadUI(":/forms/ui/mainWindow.ui");

        // get all components from widget
        this -> findChilds();

        // set playList
        this -> setPlayList();

        // attach handles.
        this -> addHandlers();
      }

      MainWindow_UI::~MainWindow_UI() {
      }

      QWidget * MainWindow_UI::getUI() const {
        return this -> widget;
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      void MainWindow_UI::findChilds() {
        this -> playList = this -> widget -> findChild < QTableView * > (
            "playList");
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
      }

      //TODO: As SilentMedia is not finished yet, this section is also not finished.
      void MainWindow_UI::setPlayList() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("collection.db");
        bool ok = db.open();

        QSqlTableModel * model = new QSqlTableModel;
        model -> setTable("collection");
        model -> setEditStrategy(QSqlTableModel::OnManualSubmit);
        model -> select();
        model -> setHeaderData(2, Qt::Horizontal, tr("Name"));
        model -> setHeaderData(3, Qt::Horizontal, tr("Artist"));
        model -> setHeaderData(4, Qt::Horizontal, tr("Album"));

        this -> playList -> setModel(model);

        // select row at all
        this -> playList -> setSelectionBehavior(QAbstractItemView::SelectRows);

        // deny change row in doubler click
        this -> playList -> setEditTriggers(QAbstractItemView::NoEditTriggers);

        // set size
        this -> playList -> setColumnWidth(1, 440);
        //        this -> playList -> setColumnWidth(3, 220);
        //        this -> playList -> setColumnWidth(4, 220);

        this -> playList -> hideColumn(0);
        //        this -> playList  -> hideColumn ( 1 );
        this -> playList -> hideColumn(5);

connect      (this -> playList, SIGNAL (doubleClicked (const QModelIndex & )), this,
          SLOT (play(const QModelIndex & )));

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

    void MainWindow_UI::play(const QModelIndex & index) {
      string fileName = index.sibling ( index.row(), 1 ).data().toString().toStdString();
      this -> player -> openFile(fileName, fileName);
      this -> player -> playFile(fileName);
    }
  }
}
}
