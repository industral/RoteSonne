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

/*

sqlite3 collection.db
CREATE TABLE collection (id INTEGER PRIMARY KEY AUTOINCREMENT, fileName VARCHAR (255), title VARCHAR (255),
artist VARCHAR (255), album VARCHAR (255), tracknum INTEGER );

sqlite3 settings.db
CREATE TABLE settings (id INTEGER PRIMARY KEY AUTOINCREMENT, var VARCHAR (255), value VARCHAR (255) );

*/

#include <sqlite.h>

SQLite::SQLite ( void ) : errmsg ( 0 ), varDbConn ( NULL )
{ }

SQLite::~SQLite ( void ) {

}

static int callback ( void * client_data, int /*argc*/, char ** argv, char ** /*azColName*/ ) {
//    for ( int i = 0; i < argc; i++ ) {
//       printf ( "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL" );
//    }
//    printf ( "\n" );

   SQLite * sql = static_cast < SQLite * > ( client_data );
//    std::cout << argv [ 0 ] << std::endl;
   sql -> setResult ( argv [ 0 ] );
   return 0;
}

bool SQLite::open ( const char * dbName, sqlite3 * &db ) {
   if ( sqlite3_open ( dbName, &db ) != SQLITE_OK ) {
      std::cout << "Can't open database: " << sqlite3_errmsg ( db ) << std::endl;
      sqlite3_close ( db );
      exit (1);
   }
   return true;
}

bool SQLite::exec ( sqlite3 * db, const char * query ) {
   if ( sqlite3_exec ( db, query, NULL, NULL, &errmsg ) != SQLITE_OK ) {
      std::cout << "SQL error: " << errmsg << std::endl;
      sqlite3_free ( errmsg );
   }
   return true;
}

void SQLite::close ( sqlite3 * db ) {
   sqlite3_close ( db );
}

void SQLite::initVal ( void ) {
   sqlite3_open ( "settings.db", &this -> varDbConn );
}

void SQLite::closeVal ( void ) {
   sqlite3_close ( this -> varDbConn );
}

std::string SQLite::getVal ( std::string val ) {
   std::string query = "SELECT value FROM settings WHERE var=\"" + val + "\"";
   if ( sqlite3_exec ( this -> varDbConn, query . c_str(), callback, this, /*&errmsg*/ NULL ) != SQLITE_OK ) {
      return "";
   } else {
      return ( this -> result );
   }
}

bool SQLite::setVal ( std::string var, std::string val ) {
   std::string query = "UPDATE settings SET VALUE=\"" + val + "\" WHERE var=\"" + var + "\"";
   if ( sqlite3_exec ( this -> varDbConn, query . c_str(), NULL, NULL, &this -> errmsg ) != SQLITE_OK ) {
      std::cout << "SQL error: " << errmsg << std::endl;
      sqlite3_free ( errmsg );
      return false;
   } else {
      return true;
   }
}

