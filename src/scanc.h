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

#ifndef __SS_SCANC_H
#define __SS_SCANC_H

#include <iostream>
#include <string>
#include <map>

#include <unistd.h> // chdir(2), stat(2)
#include <sys/types.h> // stat(2)
#include <sys/stat.h> // stat(2)

#include <app.h>
#include <dirent.h>
#include <sqlite.h>

#define SQLITE 10
#define MYSQL 20
#define POSTGRESQL 30

class Scan {
   public:
      Scan();
      ~Scan ( );
            
      bool init ( int dbType );
      void flush ( void );
      void close ( void );
      int scan ( std::string path, int level = 0 );
      std::string replace ( std::string str ) const;
   private:
      sqlite3 * db;
      SQLite * sql;

      SilentMedia::Audio * audio;
      SilentMedia::DecodedData * ddata;
//       libssoss::DSP * dsp;
//       libssoss::DSP * thdsp;
      std::map < std::string, std::string > vorbisComm;
};

#endif
