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

#include "PlayList_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      PlayList_UI::PlayList_UI(QWidget *widget) {
        this -> widget = widget;
      }

      PlayList_UI::~PlayList_UI() {
      }

      void PlayList_UI::setPlayList(QTableView *playList) {
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

        playList -> setModel(model);

        // select row at all
        playList -> setSelectionBehavior(QAbstractItemView::SelectRows);

        // deny change row in doubler click
        playList -> setEditTriggers(QAbstractItemView::NoEditTriggers);

        // set size
        playList -> setColumnWidth(1, 440);
        //        playList -> setColumnWidth(3, 220);
        //        playList -> setColumnWidth(4, 220);

        playList -> hideColumn(0);
        //        playList  -> hideColumn ( 1 );
        playList -> hideColumn(5);
      }

    // --------------------------------------------------------------------
    // Private methods
    // --------------------------------------------------------------------

    }
  }
}
