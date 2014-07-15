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

#include "TrackInfo_UI.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {

          // --------------------------------------------------------------------
          // Public methods
          // --------------------------------------------------------------------

          TrackInfo_UI::TrackInfo_UI(QWidget *widget) {
            this -> widget = widget;
          }

          TrackInfo_UI::~TrackInfo_UI() {
          }

          void TrackInfo_UI::showInfo(const string &fileId, Player *player) {
            // Set sample rate info
            this -> widget -> findChild < QLabel * > ("sampleRateInfoLabel") -> setText(
                player -> getSampleRateInfoString(fileId).c_str());

            // Set channel info
            this -> widget -> findChild < QLabel * > ("channelsInfoLabel") -> setNum(
                player -> getChannels(fileId));

            // Set bps info
            this -> widget -> findChild < QLabel * > ("bpsInfoLabel") -> setNum(
                player -> getBitsPerSample(fileId));

            // Set bit rate info
            this -> widget -> findChild < QLabel * > ("bitrateInfoLabel") -> setText(
                player -> getBitRateString(fileId).c_str());

            // Set total time info
            this -> widget -> findChild < QLabel * > ("totalTimeInfoLabel") -> setText(
                player -> getTotalTime(fileId).c_str());

            // Set file size info
            this -> widget -> findChild < QLabel * > ("fileSizeInfoLabel") -> setText(
                player -> getFileSizeString(fileId).c_str());
          }

        // --------------------------------------------------------------------
        // Private methods
        // --------------------------------------------------------------------

        }
      }
    }
  }
}
