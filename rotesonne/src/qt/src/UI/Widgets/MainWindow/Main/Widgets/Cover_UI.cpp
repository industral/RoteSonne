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

          // Singleton
          Cover_UI * Cover_UI::_coverUI = NULL;

          Cover_UI * Cover_UI::Instance() {
            if (_coverUI == NULL) {
              _coverUI = new Cover_UI();
            }
            return _coverUI;
          }

          Cover_UI::~Cover_UI() {
          }

          void Cover_UI::init(QWidget *widget) {
            this -> widget = widget;
            this -> findChilds();
          }

          void Cover_UI::setCover(const string &fileName) {
            // clear cover at start point
            this -> coverList.clear();

            // prefer cover
            string preferCover;

            if (!fileName.empty()) {
              //TODO: first should check if it file or directory
              const string currentDirectory =
                  Path(fileName).remove_filename().string();
              this -> findCover(currentDirectory);

              // if there available more than one cover, try to find font cover
              if (this -> coverList.size() > 1) {
                for (uint i = 0; i < this -> coverList.size(); ++i) {
                  const string coverFileName = this -> coverList[i];
                  if (coverFileName.find("front") != string::npos) {
                    preferCover = coverFileName;
                  }
                }

                // if we don't found front cover, set prefer cover as default
                if (preferCover.empty()) {
                  preferCover = this -> coverList[0];
                }
              }

              // if we don't found front cover, set prefer cover as default
              if (this -> coverList.size() == 1) {
                preferCover = this -> coverList[0];
              }
            }

            this -> setCoverImage(preferCover);

          }

          // --------------------------------------------------------------------
          // Private methods
          // --------------------------------------------------------------------

          Cover_UI::Cover_UI() :
            imageSize(250) {
          }

          void Cover_UI::findChilds() {
            // cover label component
            this -> cover = this -> widget -> findChild<QLabel *> ("cover");
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
            this -> cover -> setPixmap(QPixmap(":/images/nocover.png"));
          }

          bool Cover_UI::findCover(const boost::filesystem::path &path) {
            if (!exists(path)) {
              return false;
            }

            boost::filesystem::directory_iterator end_itr;
            for (boost::filesystem::directory_iterator itr(path); itr
                != end_itr; ++itr) {

              string currentPathExt =
                  boost::filesystem::path(itr->filename()).extension();
              if (is_directory(itr->status())) {
                findCover(itr->path());
                // TODO: Should be fetch extensions from a file
              } else if (!currentPathExt.compare(".png")
                  || !currentPathExt.compare(".jpg")
                  || !currentPathExt.compare(".jpeg")
                  || !currentPathExt.compare(".bmp")) {
                this -> coverList.push_back(itr->path().string());
              }
            }
            return true;
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
