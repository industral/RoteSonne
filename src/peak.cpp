/***************************************************************************
*   Copyright (C) 2008 by Alex J. Ivasyuv                                 *
*   alex@siegerstein.org.ua                                               *
*                                                                         *
*   This program is free software: you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation, either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
***************************************************************************/

// Клас для отображения уровня звука на индикаторами

#include "peak.h"

Peak::Peak ( )
	: v ( 12 ), h ( 24 )
{
}

void Peak::paintEvent ( QPaintEvent * ) {
	QPainter painter ( this );

	for ( unsigned int j = 0; j != peakLevelL . size ( ); ++j ) {
		// Рисуем последующие индикаторы со смещением по оси x
		painter . save ( );
		painter . translate ( h * j, 0 );
	
		// Если есть уровень звука, то рисуем индикаторы
		if ( peakLevelL [ j ] ) {
			for ( int i = 0; i != peakLevelL [ j ]; ++i ) {
				if ( i < 3 ) {
					painter . setBrush ( QBrush ( Qt::green, Qt::SolidPattern ) );
					painter . drawRect ( QRect ( 0, v * 8 - v * i, 15, 8 ) );
				} else if ( i < 6 ) {
					painter . setBrush ( QBrush ( Qt::yellow, Qt::SolidPattern ) );
					painter . drawRect ( QRect ( 0, v * 8 - v * i, 15, 8 ) );
				} else {
					painter . setBrush ( QBrush ( Qt::red, Qt::SolidPattern ) );
					painter . drawRect ( QRect ( 0, v * 8 - v * i, 15, 8 ) );
				}
			}
			// Если пик звука не был равень максимуму, то остальные оставшийсея рисуем
			// темным цветом
			if ( peakLevelL [ j ] < 9 ) {
				for ( int i = peakLevelL [ j ]; i != 9; ++i ) {
					drawDisablePeak ( i, painter );
				}
			}
			// Если звука нету совсем, то рисуем все темным цветом
		} else {
			for ( int i = 0; i != 9; ++i ) {
				drawDisablePeak ( i, painter );
			}
		}
		painter . restore ( );
	}
}

void Peak::drawPeak ( std::map < int, int > peakLevelL, std::map < int, int > peakLevelR ) {
	this -> peakLevelL = peakLevelL;
	this -> peakLevelR = peakLevelR;
	update ( );
}

// рисует все темным цветом
void Peak::drawDisablePeak ( int i, QPainter &painter ) {
	if ( i < 3 ) {
		painter . setBrush ( QBrush ( Qt::darkGreen, Qt::SolidPattern ) );
		painter . drawRect ( QRect ( 0, v * 8 - v * i, 15, 8 ) );
	} else if ( i < 6 ) {
		painter . setBrush ( QBrush ( Qt::darkYellow, Qt::SolidPattern ) );
		painter . drawRect ( QRect ( 0, v * 8 - v * i, 15, 8 ) );
	} else {
		painter . setBrush ( QBrush ( Qt::darkRed, Qt::SolidPattern ) );
		painter . drawRect ( QRect ( 0, v * 8 - v * i, 15, 8 ) );
	}
}
