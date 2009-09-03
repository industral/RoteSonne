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
              cfg(Configuration::Instance()) {
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
                  qDebug() << "New root: " << itr->path().string().c_str();
                  scanPlayListFiles(itr->path());
                  // TODO: Should be fetch extensions fom file
                } else if (!currentPathExt.compare(".xspf")) {
                  qDebug() << "New item: " << itr->path().string().c_str();
                  //this -> fileList.push_back(itr->path().string());
                }
              }
              return true;
            }

            void Library_UI::setPlayList() {
              QString path = this -> cfg -> getPlayListFolderPath();
              this -> scanPlayListFiles(path.toStdString().c_str());

              this -> treeWidget -> setColumnCount(1);
              QList <QTreeWidgetItem *> items;

              QTreeWidgetItem * i = new QTreeWidgetItem(QStringList("asd"));

              i -> setData(itemPlayListNumber, Qt::DisplayRole, QVariant(
                  itemPlayListDefinition));
              i -> setData(itemPlayListFilePathNumber, Qt::DisplayRole,
                  QVariant("YTRY"));

              items.append(i);

              //              for (int i = 0; i < 10; ++i) {
              //                items.append(new QTreeWidgetItem(QStringList(
              //                    QString("item: %1").arg(i))));
              //              }

              this -> playListQTreeWidgetItem -> addChildren(items);

              connect(this -> treeWidget,
                  SIGNAL(itemClicked(QTreeWidgetItem *, int)),
                  SLOT(eventHandler(QTreeWidgetItem *, int)));
            }

            void Library_UI::handlePlayList() {

            }

            // --------------------------------------------------------------------
            // Private slots
            // --------------------------------------------------------------------

            void Library_UI::eventHandler(QTreeWidgetItem * item, int column) {
              qDebug() << item -> text(column);
              qDebug()
                  << item -> data(itemPlayListNumber, Qt::DisplayRole).toString();
            }

          }
        }
      }
    }
  }
}
