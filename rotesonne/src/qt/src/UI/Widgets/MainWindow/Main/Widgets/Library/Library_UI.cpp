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

            // --------------------------------------------------------------------
            // Public methods
            // --------------------------------------------------------------------

            Library_UI::Library_UI(QWidget *widget) {
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
                  = this -> widget -> findChild < QTreeWidget * > ("treeWidget");
            }

            void Library_UI::setExpand() {
              // expand root
              this -> treeWidget -> expandItem(
                  this -> treeWidget -> topLevelItem(0));

              // expand music
              this -> treeWidget -> expandItem(
                  this -> treeWidget -> topLevelItem(0) -> child(0));

              // expand playlist
              this -> treeWidget -> expandItem(
                  this -> treeWidget -> topLevelItem(0) -> child(0) -> child(0));
            }

            void Library_UI::setPlayList() {

              this -> treeWidget -> setColumnCount(1);
              QList < QTreeWidgetItem * > items;
              for (int i = 0; i < 10; ++i)
                items.append(new QTreeWidgetItem((QTreeWidget*) 0, QStringList(
                    QString("item: %1").arg(i))));
              this -> treeWidget -> topLevelItem(0) -> child(0) -> child(0) -> addChildren(
                  items);
            }

          }
        }
      }
    }
  }
}
