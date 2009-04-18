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

      Collection_UI::Collection_UI(Collection_UI *p) {
        this -> self = p;

        // load UI widget
        this -> widget = LoadUI::loadUI(":/forms/ui/Collection.ui");

        // attach handlers
        this -> addHandlers();
      }

      Collection_UI::~Collection_UI() {
      }

      void Collection_UI::show() {
        this -> widget -> show();
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      void Collection_UI::addHandlers() {
        connect(this -> widget -> findChild < QDialogButtonBox * > ("closeButton"),
            SIGNAL(clicked(QAbstractButton*)), widget, SLOT(close()));

        connect(this -> widget -> findChild < QDialogButtonBox * > ("closeButton"),
            SIGNAL(clicked(QAbstractButton*)), this, SLOT(close()));

        // add "Browse" handler
        connect(this -> widget -> findChild < QPushButton * > ("browseButton"),
            SIGNAL(clicked()), this, SLOT(openDialog()));

        // add "Scan" handler
        connect(this -> widget -> findChild < QPushButton * > ("scanButton"),
            SIGNAL(clicked()), this, SLOT(scanCollection()));

        // add "Collection" handler
        //    connect(this -> widget -> findChild < QAction * > ("actionCollection"),
        //        SIGNAL(triggered()), this, SLOT(collectionPreferences()));
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
        cout << "asd" << endl;
        QApplication::aboutQt();
      }

      void Collection_UI::scanCollection() {
        cout << "scan" << endl;
        //      QWidget *widget = LoadUI::loadUI(":/forms/ui/aboutRoteSonne.ui");
        //      widget -> show();
      }

      void Collection_UI::close() {
        this -> callDestructor();
      }

    //
    //  void Collection_UI::collectionPreferences() {
    //    QWidget *widget = LoadUI::loadUI(":/forms/ui/Collection.ui");
    //    widget -> show();
    //  }
    }
  }
}
