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

#include "Cover_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {

          // --------------------------------------------------------------------
          // Public methods
          // --------------------------------------------------------------------

          Cover_UI::Cover_UI() :
            imageSize(250) {

          }

          Cover_UI::~Cover_UI() {
          }

          void Cover_UI::init(QWidget *widget) {
            this -> widget = widget;
            this -> findChilds();
          }

          void Cover_UI::setCover(const string &fileName) {
            this -> setCoverImage(this -> findCover(fileName));
          }

          // --------------------------------------------------------------------
          // Private methods
          // --------------------------------------------------------------------

          void Cover_UI::findChilds() {
            // cover label component
            this -> cover = this -> widget -> findChild < QLabel * > ("cover");
          }

          void Cover_UI::setCoverImage(const string &image) {
            // if cover present
            if (!image.empty()) {
              QPixmap pixmap(image.c_str());
              this -> imageTunning(pixmap);

              this -> cover -> setPixmap(pixmap);
            } else { // if not cover found
              this -> cover -> setFixedSize(this -> imageSize,
                  this -> imageSize);
              this -> setNoCoverImage();
            }
          }

          void Cover_UI::setNoCoverImage() {
            this -> cover -> setPixmap(QPixmap(":/images/images/nocover.png"));
          }

          string Cover_UI::findCover(const string &fileName) const {
            // directory location current file
            string directory = Path(fileName).remove_filename().string();

            boost::filesystem::directory_iterator end_itr;
            for (boost::filesystem::directory_iterator itr(directory); itr
                != end_itr; ++itr) {
              if (boost::filesystem::is_regular_file((itr->status()))) {
                string fileName = Path(itr->filename()).string();
                string fileExtension = Path(fileName).extension();

                // transform to upper case for case insensitive searching
                transform(fileName.begin(), fileName.end(), fileName.begin(),
                    ::toupper);

                if (fileName.find("COVER", 0) != string::npos
                    && (fileExtension.compare(".png") || fileExtension.compare(
                        ".jpg"))) {
                  return itr->path().string();
                }
              }
            }
            return "";
          }

          void Cover_UI::imageTunning(const QPixmap &pix) {
            // get image width & height
            int width = pix.width();
            int height = pix.height();

            int widthK = width;
            int heightK = height;

            /*
             * Change image size if it more that imageSize
             */

            // if one of dimension (height/width) more that imageSize, resize it
            if ((std::max(width, height)) > (this -> imageSize)) {
              double k = (std::max(width, height)) / (this -> imageSize);
              widthK = width / k;
              heightK = height / k;
            }
            // set fixed size
            this -> cover -> setFixedSize(widthK, heightK);
          }

        }
      }
    }
  }
}
