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

#include <QApplication>

#include "UI/Widgets/MainWindow/MainWindow_UI.hpp"
#include "StartUp.hpp"

using namespace RoteSonne;
using namespace RoteSonne::UI::Widgets::MainWindow;

int main(int argc, char **argv) {
  StartUP * startup = new StartUP();
  if (!startup -> init()) {
    qDebug() << "Can't startup application";
    return -1;
  }

  // set encoding to UTF-8
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForTr(codec);
  QTextCodec::setCodecForLocale(codec);
  QTextCodec::setCodecForCStrings(codec);

  QApplication app(argc, argv);
  // force use cleanlooks style, as GTK+ has wrong rendering
//  app.setStyle(new QCleanlooksStyle());

  MainWindow_UI * mainWindow = MainWindow_UI::Instance();
  mainWindow -> init();
  QWidget *widget = mainWindow -> getUI();

  if (widget == NULL) {
    return -1;
  }

  widget -> show();
  int r = app.exec();

  delete mainWindow;
  delete widget;

  return r;
}
