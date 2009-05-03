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

#include "Collection_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      Collection_UI::Collection_UI(Collection_UI *p, QTableView *playList) {
        this -> playList = playList;

        this -> self = p;
        this -> timer = new QTimer(this);

        this -> collectionDb = new Collection();
        this -> playListUI = PlayList_UI::Instance();

        // load UI widget
        this -> widget = LoadUI::loadUI(":/forms/ui/Collection.ui");

        // find all components in widget
        this -> findChilds();

        // attach handlers
        this -> addHandlers();

        this -> collectionPathLineEdit -> setText("/data/music");
      }

      Collection_UI::~Collection_UI() {
        delete this -> collectionDb;
        this -> collectionDb = NULL;

        //        delete this -> playListUI;
        //        this -> playListUI = NULL;
      }

      void Collection_UI::show() {
        this -> widget -> show();
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      void Collection_UI::findChilds() {
        this -> closeButton
            = this -> widget -> findChild < QDialogButtonBox * > ("closeButton");

        this -> browseButton = this -> widget -> findChild < QPushButton * > (
            "browseButton");

        this -> scanButton = this -> widget -> findChild < QPushButton * > (
            "scanButton");

        this -> progressBar = this -> widget -> findChild < QProgressBar * > (
            "progressBar");

        this -> collectionPathLineEdit = this -> widget -> findChild <
            QLineEdit * > ("collectionPathLineEdit");
      }

      void Collection_UI::addHandlers() {
        // add "Close" handle
        // close widget
        connect(this -> closeButton,
            SIGNAL(clicked(QAbstractButton*)), widget, SLOT(close()));

        // free resource on close
        connect(this -> closeButton,
            SIGNAL(clicked(QAbstractButton*)), this, SLOT(close()));

        // add "Browse" handler
        connect(this -> browseButton, SIGNAL(clicked()), this, SLOT(
            openDialog()));

        // add "Scan" handler
        connect(this -> scanButton, SIGNAL(clicked()), this, SLOT(
            scanCollection()));

        // Update process bar position every n seconds
        connect(this -> timer, SIGNAL(timeout()), this,
            SLOT(updateProcessBar()));
      }

      void Collection_UI::callDestructor() {
        cout << "Destruct" << endl;
        delete this -> self;
        this -> self = NULL;
      }

      // --------------------------------------------------------------------
      // Slots
      // --------------------------------------------------------------------

      void Collection_UI::openDialog() {
        QFileDialog fileDialog;

        string collectionFolder = ((fileDialog.getExistingDirectory(this, tr(
            "Select collection folder"), QDir::homePath(),
            QFileDialog::ShowDirsOnly)).toStdString());

        this -> collectionPathLineEdit -> setText((collectionFolder).c_str());
      }

      void Collection_UI::scanCollection() {
        this -> playListUI -> dropPlayList();

        string collectionPath =
            this -> collectionPathLineEdit -> text().toStdString();

        this -> collectionDb -> open("collection.db");
        this -> collectionDb -> scan(collectionPath);

        this -> timer -> start();
      }

      void Collection_UI::close() {
        this -> callDestructor();
      }

      void Collection_UI::updateProcessBar() {
        // Waiting end
        if (!this -> collectionDb -> getStatus()) {
          this -> timer -> stop();
          this -> playListUI -> setPlayList(this -> playList);
        }
        progressBar -> setValue(this -> collectionDb -> getProcess());
      }

    }
  }
}
