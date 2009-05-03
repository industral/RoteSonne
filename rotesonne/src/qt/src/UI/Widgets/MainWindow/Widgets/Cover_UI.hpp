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

#ifndef _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_COVER_HPP_
#define _ROTESONNE_UI_WIDGETS_MAINWINDOW_WIDGETS_COVER_HPP_

#include <iostream>
#include <string>

// include require Qt headers
#include <QLabel>

// boost filesystem
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

typedef boost::filesystem::path Path;

using namespace std;

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          class Cover_UI {
            public:
              /**
               * Default constructor.
               */
              Cover_UI();

              /**
               * Default destructor.
               */
              ~Cover_UI();

              void init(QWidget *widget);
              void setCover(const string &fileName);
            private:
              // to avoid cast to double
              const double imageSize; // image size

              QWidget * widget;
              QLabel * cover;

              void findChilds();

              /**
               * Searching a cover file in current file directory.
               * @param[in] fileName Track file name.
               * @return full path to cover image.
               */
              string findCover(const string &fileName) const;
              void setCoverImage(const string &image);
              void imageTunning(const QPixmap &pix);

          };
        }
      }
    }
  }
}

#endif
