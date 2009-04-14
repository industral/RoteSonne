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

#ifndef SQLITE_H
#define SQLITE_H

#include <iostream>
#include <stdlib.h> // exit ( 3)
#include <sqlite3.h>

class SQLite {
   public:
      SQLite ( void );
      ~SQLite ( void );

      bool open ( const char * dbName, sqlite3 * &db );
      bool exec ( sqlite3 * db, const char * query );
      void close ( sqlite3 * db );

      void initVal ( void );
      void closeVal ( void );

      void setResult ( std::string result ) { this -> result = result; }
      std::string getVal ( std::string val );
      bool setVal ( std::string var, std::string val );
   private:
      char * errmsg;
      sqlite3 * varDbConn;
      std::string result;
};

#endif
