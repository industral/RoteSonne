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

#ifndef PEAK_H
#define PEAK_H

#include <QtGui>
#include <iostream>

class Peak : public QWidget
{
	public:
		Peak ( );
		
		void drawPeak ( std::map < int, int > peakLevelL, std::map < int, int > peakLevelR );
		void drawDisablePeak ( int i, QPainter &painter );
	protected:
		void paintEvent ( QPaintEvent * );
	private:
		int v; // разстояние по вертикали
		int h; // разтояние по горизонтали
		QWidget *dmw;
		std::map < int, int > peakLevelL;
		std::map < int, int > peakLevelR;
};

#endif
