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

                this -> mainWindow = MainWindow_UI::Instance();
              }

              void TrackList_UI::playFirstTrack() {
                // get index for 1st track
                QModelIndex index = this -> model -> index(0, 0);

                this -> trackListComponent -> selectRow(0);

                this -> mainWindow -> play(index);
              }

              void TrackList_UI::setPlayList() {
                this -> model = new QSqlTableModel();

                this -> model -> setTable("collection");
                this -> model -> setFilter(this -> filter);
                this -> model -> select();

                this -> model -> setHeaderData(2, Qt::Horizontal, QObject::tr("Track"));
                this -> model -> setHeaderData(3, Qt::Horizontal, QObject::tr("Name"));
                this -> model -> setHeaderData(4, Qt::Horizontal, QObject::tr("Artist"));
                this -> model -> setHeaderData(5, Qt::Horizontal, QObject::tr("Album"));

                this -> model -> setEditStrategy(QSqlTableModel::OnManualSubmit);

                this -> trackListComponent -> setItemDelegateForColumn(3, new ItemDelegate);
                this -> trackListComponent -> setModel(this -> model);

                // select row at all
                this -> trackListComponent -> setSelectionBehavior(QAbstractItemView::SelectRows);

                // deny change row in doubler click
                this -> trackListComponent -> setEditTriggers(QAbstractItemView::NoEditTriggers);

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

              void TrackList_UI::refresh(const QModelIndex & index) {
                this -> trackListComponent -> update(index);
                //                this -> dropPlayList();
                //                this -> setPlayList();
              }

              void TrackList_UI::setFilter(const QString &filter) {
                this -> filter = filter;
              }

              void TrackList_UI::dropPlayList() {
                this -> model -> clear();
                delete this -> model;
                this -> model = NULL;
              }

              QModelIndex & TrackList_UI::getSelectedIndex() {
                return this -> selectedIndex;
              }

              QModelIndex & TrackList_UI::getPlayedIndex() {
                //TODO: variable sucks
                return this -> playedIndex;
              }

              int TrackList_UI::getRowCount() {
                return this -> model -> rowCount();
              }

              // --------------------------------------------------------------------
              // Private methods
              // --------------------------------------------------------------------

              TrackList_UI::TrackList_UI() :
                model(NULL) {
                this -> cfg = Configuration::Instance();
                this -> collectionDb = new ::RoteSonne::Collection();
                this -> openDbConnection();
              }

              void TrackList_UI::findChilds() {
                // track list
                this -> trackListComponent = this -> widget -> findChild<QTableView *> ("trackList");
              }

              bool TrackList_UI::openDbConnection() {
                this -> db = QSqlDatabase::addDatabase("QSQLITE");

                QString playListlocation = this -> cfg -> getPlayListFolderPath();

                QString defaultCollection = playListlocation + "/" + DEFAULT_PLAYLIST + DB_EXT;

                bool createDb = false;

                const boost::filesystem::path &path = defaultCollection.toStdString();
                if (!exists(path)) {
                  createDb = true;
                }

                this -> db.setDatabaseName(defaultCollection);

                bool ok = this -> db.open();

                if (!ok) {
                  qDebug() << "Error: Unable to open database `" << defaultCollection << "'";
                }

                if (createDb) {
                  this -> collectionDb -> open();
                  this -> collectionDb -> createDbStructure();
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
                connect(this -> trackListComponent, SIGNAL (doubleClicked (const QModelIndex & )), this,
                    SLOT (setPlayingIndex(const QModelIndex & )));
                connect(this -> trackListComponent, SIGNAL (doubleClicked (const QModelIndex & )), this,
                    SLOT (play(const QModelIndex & )));
                connect(this -> trackListComponent, SIGNAL (pressed(const QModelIndex &)), this,
                    SLOT (initLocation(const QModelIndex &)));
                connect(this -> trackListComponent, SIGNAL (pressed(const QModelIndex &)), this,
                    SLOT (showInfo(const QModelIndex &)));
                connect(this -> trackListComponent, SIGNAL (pressed(const QModelIndex &)), this,
                    SLOT (activateEmelents(const QModelIndex &)));

                connect(this -> trackListComponent, SIGNAL (doubleClicked(const QModelIndex &)), this,
                    SLOT (setView(const QModelIndex &)));
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

              void TrackList_UI::setView(const QModelIndex & index) {
                //                qDebug() << "in view ";
                //            QFont font = QFont("Arial", 17, QFont::Bold);
                //            QVariant * v = new QVariant(font);
                //            //QString s = "asdasd";
                //            v -> setValue("asd");
                //            this -> model -> setData(m, &v);

                //                                this -> model->setData(m, qVariantFromValue(QColor(Qt::red)));

                //                qDebug() << this -> model->setData(m, qVariantFromValue(QColor(Qt::red)), Qt::TextColorRole);

                //this -> model->setData(m, QString("ads"), Qt::TextSelectableByMouse);
                //                QStyleOptionViewItem viewOption;
                //                viewOption.font.setBold(true);
                //                QItemDelegate::paint(painter, viewOption, index);

              }

              void TrackList_UI::play(const QModelIndex &index) {
                this -> mainWindow -> play(index);
              }

              void TrackList_UI::initLocation(const QModelIndex & index) {
                this -> selectedIndex = index;
              }

              void TrackList_UI::showInfo(const QModelIndex &index) {
                this -> mainWindow -> showInfo(index);
              }

              void TrackList_UI::activateEmelents(const QModelIndex &index) {
                this -> mainWindow -> activateEmelents(index);
              }

              void TrackList_UI::setPlayingIndex(const QModelIndex &index) {
                this -> playedIndex = index;
              }

            }
          }
        }
      }
    }
  }
}
