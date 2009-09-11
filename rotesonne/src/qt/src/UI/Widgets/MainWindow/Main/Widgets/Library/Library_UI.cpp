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

#include "Library_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace Library {

            const QString Library_UI::itemPlayListDefinition = QString(
                "PLAYLIST");
            const int Library_UI::itemPlayListNumber = 500;
            const int Library_UI::itemPlayListFilePathNumber = 501;

            // --------------------------------------------------------------------
            // Public methods
            // --------------------------------------------------------------------

            Library_UI::Library_UI() {
            }

            Library_UI::Library_UI(QWidget *widget) :
              cfg(Configuration::Instance()), pl(
                  new SilentMedia::Media::PlayList::PlayList) {
              Library_UI();

              this -> widget = widget;

              this -> findChilds();
              this -> setExpand();
              this -> setPlayList();
            }

            Library_UI::~Library_UI() {
            }

            // --------------------------------------------------------------------
            // Private methods
            // --------------------------------------------------------------------

            void Library_UI::findChilds() {
              this -> treeWidget
                  = this -> widget -> findChild <QTreeWidget *> ("treeWidget");
            }

            void Library_UI::defineRoots() {
              this -> playListQTreeWidgetItem
                  = this -> treeWidget -> topLevelItem(0) -> child(0) -> child(
                      0);
            }

            void Library_UI::setExpand() {
              this -> defineRoots();

              // expand root
              this -> treeWidget -> expandItem(
                  this -> treeWidget -> topLevelItem(0));

              // expand music
              this -> treeWidget -> expandItem(
                  this -> treeWidget -> topLevelItem(0) -> child(0));

              // expand playlist
              this -> treeWidget -> expandItem(this -> playListQTreeWidgetItem);
            }

            bool Library_UI::scanPlayListFiles(
                const boost::filesystem::path &path) {
              if (!exists(path)) {
                return false;
              }

              boost::filesystem::directory_iterator end_itr;
              for (boost::filesystem::directory_iterator itr(path); itr
                  != end_itr; ++itr) {
                string currentPathExt =
                    boost::filesystem::path(itr->filename()).extension();

                if (is_directory(itr->status())) {
                  scanPlayListFiles(itr->path());
                  // TODO: Should be fetch extensions fom file
                } else if (!currentPathExt.compare(".xspf")) {
                  this -> playListList.push_back(itr->path().string().c_str());
                }
              }
              return true;
            }

            void Library_UI::setPlayList() {
              const QString playListFolderPath =
                  this -> cfg -> getPlayListFolderPath();

              this -> scanPlayListFiles(
                  playListFolderPath.toStdString().c_str());

              this -> treeWidget -> setColumnCount(1);
              QList <QTreeWidgetItem *> items;

              for (int i = 0; i < this -> playListList.size(); ++i) {
                const QString playListPath = this -> playListList[i];
                const QString playListName = playListPath.split(
                    playListFolderPath)[1];

                QTreeWidgetItem * i = new QTreeWidgetItem(QStringList(
                    playListName));

                i -> setData(itemPlayListNumber, Qt::DisplayRole, QVariant(
                    itemPlayListDefinition));
                i -> setData(itemPlayListFilePathNumber, Qt::DisplayRole,
                    QVariant(playListPath));

                items.append(i);
              }

              this -> playListQTreeWidgetItem -> addChildren(items);

              connect(this -> treeWidget,
                  SIGNAL(itemClicked(QTreeWidgetItem *, int)),
                  SLOT(eventHandler(QTreeWidgetItem *, int)));
            }

            void Library_UI::handlePlayList(QTreeWidgetItem * item, int column) {
              QString playListFilePath = item -> data(
                  itemPlayListFilePathNumber, Qt::DisplayRole).toString();

              this -> pl -> openPlayList(playListFilePath.toStdString().c_str());
              list <string> l = this -> pl -> getPlayList();

              for (list <string>::iterator it = l.begin(); it != l.end(); it++) {
                cout << *it << endl;
              }

              this -> pl -> close();

            }

            // --------------------------------------------------------------------
            // Private slots
            // --------------------------------------------------------------------

            void Library_UI::eventHandler(QTreeWidgetItem * item, int column) {
              QString playList = item -> data(itemPlayListNumber,
                  Qt::DisplayRole).toString();

              if (!playList.compare(itemPlayListDefinition)) {
                this -> handlePlayList(item, column);
              }

            }

          }
        }
      }
    }
  }
}
