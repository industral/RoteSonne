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

#ifndef _ROTESONNE_UI_WIDGETS_PREFERENCES_PREFERENCES_HPP_
#define _ROTESONNE_UI_WIDGETS_PREFERENCES_PREFERENCES_HPP_

#include <include.hpp>

// include UI loader
#include <UI/LoadUI.hpp>

using namespace std;

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace Preferences {
        /**
         * Collection preferences/scan widget.
         * Simple example how to use. To show this widget, need just create object
         * and call show method:
         * @code
         * Preferences_UI *collection = new Preferences_UI(collection);
         * collection -> show();
         * @endcode
         * Thats all.
         */
        class Preferences_UI: public QWidget {
          Q_OBJECT
          public:

            /**
             * Main constructor. Should be used to create widget instance.
             */
            Preferences_UI();

            /**
             * Standard destructor.
             */
            ~Preferences_UI();

            /**
             * Show widget. Call this function to provide appearing widget.
             * @code
             * collection -> show();
             * @endcode
             */
            void show();

            private slots:
            void accept();
            void reject();

          private:
            // self pointer. Need to delete myself.
            Preferences_UI *self;

            // Preferences widget
            QWidget * widget;
            QDialogButtonBox * dialogButtonBox;

            void findChilds();
            void addHandlers();
        };
      }
    }
  }
}

#endif
