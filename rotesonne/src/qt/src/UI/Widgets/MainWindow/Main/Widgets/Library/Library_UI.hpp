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

#ifndef _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_LIBRARY_LIBRARY_UI_HPP_
#define _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_LIBRARY_LIBRARY_UI_HPP_

// include require Qt headers
#include <QtGui>

#include <string>

// boost filesystem
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

//TODO: Fix this shit!
#include "../../../../../../Configuration.hpp"

#include <libsml/all.hpp>

using namespace std;

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace Library {
            class Library_UI: public QWidget {
              Q_OBJECT
              public:

                Library_UI();

                /**
                 * Default constructor.
                 */
                Library_UI(QWidget *widget);

                /**
                 * Default destructor.
                 */
                ~Library_UI();
              private:
                void findChilds();
                void setPlayList();
                void defineRoots();
                void setExpand();
                bool scanPlayListFiles(const boost::filesystem::path &path);
                void handlePlayList(QTreeWidgetItem * item, int column);

                static const QString itemPlayListDefinition;
                static const int itemPlayListNumber;
                static const int itemPlayListFilePathNumber;

                QWidget * widget;
                QTreeWidget *treeWidget;
                Configuration * cfg;

                SilentMedia::Media::PlayList::PlayList * pl;

                QTreeWidgetItem * playListQTreeWidgetItem;
                QList <QString> playListList;

              private slots:
                void eventHandler(QTreeWidgetItem *, int);
            };
          }
        }
      }
    }
  }
}

#endif
