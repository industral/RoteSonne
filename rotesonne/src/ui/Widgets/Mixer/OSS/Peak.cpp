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

// Клас для отображения уровня звука на индикаторами

#include "Peak.hpp"

Peak::Peak() :
  v(12), h(24) {
}

void Peak::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  for (unsigned int j = 0; j != peakLevelL . size(); ++j) {
    // Рисуем последующие индикаторы со смещением по оси x
    painter . save();
    painter . translate(h * j, 0);

    // Если есть уровень звука, то рисуем индикаторы
    if (peakLevelL[j]) {
      for (int i = 0; i != peakLevelL[j]; ++i) {
        if (i < 3) {
          painter . setBrush(QBrush(Qt::green, Qt::SolidPattern));
          painter . drawRect(QRect(0, v * 8 - v * i, 15, 8));
        } else if (i < 6) {
          painter . setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
          painter . drawRect(QRect(0, v * 8 - v * i, 15, 8));
        } else {
          painter . setBrush(QBrush(Qt::red, Qt::SolidPattern));
          painter . drawRect(QRect(0, v * 8 - v * i, 15, 8));
        }
      }
      // Если пик звука не был равень максимуму, то остальные оставшийсея рисуем
      // темным цветом
      if (peakLevelL[j] < 9) {
        for (int i = peakLevelL[j]; i != 9; ++i) {
          drawDisablePeak(i, painter);
        }
      }
      // Если звука нету совсем, то рисуем все темным цветом
    } else {
      for (int i = 0; i != 9; ++i) {
        drawDisablePeak(i, painter);
      }
    }
    painter . restore();
  }
}

void Peak::drawPeak(map <int, int> peakLevelL, map <int, int> peakLevelR) {
  this -> peakLevelL = peakLevelL;
  this -> peakLevelR = peakLevelR;
  update();
}

// рисует все темным цветом
void Peak::drawDisablePeak(int i, QPainter &painter) {
  if (i < 3) {
    painter . setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
    painter . drawRect(QRect(0, v * 8 - v * i, 15, 8));
  } else if (i < 6) {
    painter . setBrush(QBrush(Qt::darkYellow, Qt::SolidPattern));
    painter . drawRect(QRect(0, v * 8 - v * i, 15, 8));
  } else {
    painter . setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
    painter . drawRect(QRect(0, v * 8 - v * i, 15, 8));
  }
}
