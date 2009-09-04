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

#include "TrackList_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace PlayList {
            namespace TrackList {

              // Singleton
              TrackList_UI * TrackList_UI::_playListUI = NULL;

              TrackList_UI * TrackList_UI::Instance() {
                if (_playListUI == NULL) {
                  _playListUI = new TrackList_UI();
                }
                return _playListUI;
              }

              // --------------------------------------------------------------------
              // Public methods
              // --------------------------------------------------------------------

              TrackList_UI::~TrackList_UI() {
                this -> closeDbConnection();
              }

              void TrackList_UI::init(QWidget *widget) {
                this -> widget = widget;
                this -> findChilds();
                this -> addHandlers();
              }

              void TrackList_UI::setPlayList() {
                this -> model = new CustomQSqlTableModel();

                this -> model -> setTable("collection");
                this -> model -> setFilter(this -> filter);
                this -> model -> select();

                this -> model -> setHeaderData(2, Qt::Horizontal, QObject::tr(
                    "Track"));
                this -> model -> setHeaderData(3, Qt::Horizontal, QObject::tr(
                    "Name"));
                this -> model -> setHeaderData(4, Qt::Horizontal, QObject::tr(
                    "Artist"));
                this -> model -> setHeaderData(5, Qt::Horizontal, QObject::tr(
                    "Album"));

                this -> model -> setEditStrategy(QSqlTableModel::OnManualSubmit);

                this -> trackListComponent -> setModel(this -> model);

                // select row at all
                this -> trackListComponent -> setSelectionBehavior(
                    QAbstractItemView::SelectRows);

                // deny change row in doubler click
                this -> trackListComponent -> setEditTriggers(
                    QAbstractItemView::NoEditTriggers);

                // odd
                this -> trackListComponent -> setAlternatingRowColors(true);

                // hide column
                this -> trackListComponent -> hideColumn(0); // id
                this -> trackListComponent -> hideColumn(1); // filename

                // set size
                this -> trackListComponent -> setColumnWidth(2, 50); // track
                this -> trackListComponent -> setColumnWidth(3, 320); // name
                this -> trackListComponent -> setColumnWidth(4, 150); // artist
                this -> trackListComponent -> setColumnWidth(5, 150); // album

                // remove vertical stuff
                this -> trackListComponent -> verticalHeader() -> hide();
              }

              void TrackList_UI::setFilter(const QString &filter) {
                this -> filter = filter;
              }

              void TrackList_UI::dropPlayList() {
                this -> model -> clear();
                delete this -> model;
                this -> model = NULL;
              }

              // --------------------------------------------------------------------
              // Private methods
              // --------------------------------------------------------------------

              TrackList_UI::TrackList_UI() :
                model(NULL) {
                this -> openDbConnection();
              }

              void TrackList_UI::findChilds() {
                // track list
                this -> trackListComponent = this -> widget -> findChild <
                    QTableView *> ("trackList");
              }

              bool TrackList_UI::openDbConnection() {
                this -> db = QSqlDatabase::addDatabase("QSQLITE");
                this -> db.setDatabaseName("collection.db");
                bool ok = this -> db.open();

                if (!ok) {
                  cerr << "Error: Unable to open database `" << "collection.db"
                      << "'" << endl;
                }
                return ok;
              }

              void TrackList_UI::closeDbConnection() {
                this -> db = QSqlDatabase::database();

                if (this -> db.isOpen()) {
                  this -> db.close();
                }

                if (this -> db.isValid()) {
                  this -> db.removeDatabase(this -> db.connectionNames()[0]);
                }
              }

              void TrackList_UI::addHandlers() {
connect              ( this -> trackListComponent, SIGNAL (doubleClicked(const QModelIndex &)), this, SLOT (setView(const QModelIndex &)));
            }

            //            QVariant TrackList_UI::data(const QModelIndex &index, int role) const {
            //              //            QVariant value = QSqlQueryModel::data(index, role);
            //              //            if (value.isValid() && role == Qt::DisplayRole) {
            //              //              if (index.column() == 3)
            //              //              return value.toString().prepend("#");
            //              //              else if (index.column() == 3)
            //              //              return value.toString().toUpper();
            //              //            }
            //              //            if (role == Qt::TextColorRole && index.column() == 3)
            //              //            return qVariantFromValue(QColor(Qt::blue));
            //              //            return value;
            //              qDebug() << "ads";
            //            }

            // --------------------------------------------------------------------
            // Private slots
            // --------------------------------------------------------------------

            void TrackList_UI::setView(const QModelIndex & m) {
              //            qDebug() << "in view ";
              //            QFont font = QFont("Arial", 17, QFont::Bold);
              //            QVariant * v = new QVariant(font);
              //            //QString s = "asdasd";
              //            v -> setValue("asd");
              //            this -> model -> setData(m, &v);

              this -> model->setData(m, qVariantFromValue(QColor(Qt::red)));
              //this -> model->setData(m, QString("ads"), Qt::TextSelectableByMouse);

            }
          }
        }
      }
    }
  }
}
}
