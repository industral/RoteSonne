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
   ВНИМАНИЕ:
   Используем stat(2) а не d_type из dirent.h, потому что этот параметр может не работать
   на всех файловых системах, в частности это касается raiserfs.
*/

#include "scanc.h"

static int filter ( const struct dirent * dir ) {
   if ( ( dir -> d_name [ 0 ] == '.' ) && ( dir -> d_name [ 1 ] == 0 ) ) { return 0; }
   if ( ( dir -> d_name [ 0 ] == '.' ) && ( dir -> d_name [ 1 ] == '.' ) && ( dir -> d_name [ 2 ] == 0 ) ) { return 0; }
   return 1;
}

Scan::Scan ( void ) : db ( NULL ), sql ( NULL )
{ }


Scan::~Scan ( void ) { }

bool Scan::init ( int dbType ) {
   this -> audio = SilentMedia::Audio::Instance();
   this -> ddata = SilentMedia::DecodedData::Instance();
   this -> sql = new SQLite;
   if ( dbType == SQLITE ) {
      sql -> open ( "collection.db", this -> db );
   }
   return true;
}

void Scan::flush ( void ) {
   sql -> exec ( this -> db, "DROP TABLE collection" );
   sql -> exec ( this -> db, "CREATE TABLE collection (id INTEGER PRIMARY KEY AUTOINCREMENT, fileName VARCHAR (255), title VARCHAR (255), artist VARCHAR (255), album VARCHAR (255), tracknum INTEGER )" );
}

void Scan::close ( void ) {
   sql -> close ( this -> db );
}

int Scan::scan ( std::string path, int level ) {
   if ( !path . empty() ) {

      if ( chdir ( path . c_str() ) == -1 ) {
//  if (errno == )
         std::cout << "Error to chdir() in Scan::scan() " << std::endl;
      }

      struct dirent **namelist;
      struct stat statBuf;

      int n = scandir ("./", &namelist, filter, alphasort);

      if (n >= 0) {
         int cnt;
         for ( cnt = 0; cnt < n; ++cnt ) {

            char currPathBuf [255];
            if ( getcwd ( currPathBuf, 254 ) == NULL ) {
               perror ( "getcwd error" );
            }

            std::string fullPathFileName_;
            std::string fullPathFileName = currPathBuf;

            fullPathFileName += "/";
            fullPathFileName_ = fullPathFileName;
            fullPathFileName += namelist [ cnt ] -> d_name;

            if ( stat ( fullPathFileName.c_str(), &statBuf ) != 0 ) {
               std::cout << "Error in stat() for file " << fullPathFileName << std::endl;
            }

            if ( statBuf.st_mode & S_IFDIR ) { // if dir
               scan ( namelist [ cnt ] -> d_name, level + 1 );
            }

            if ( statBuf.st_mode & S_IFREG ) { // if file
               std::string fileName = namelist [ cnt ] -> d_name;
               std::string ext = fileName . substr ( ( ( fileName . find (".", fileName . size() - 5 ) ) + 1 ), fileName . size() );
               if ( ext == "wav" || ext == "ogg" || ext == "flac" || ext == "wv" ) {

                  fullPathFileName_ += this -> replace ( fileName );

                  std::string id = fullPathFileName;
                  this -> audio -> openFile ( fullPathFileName, id );
                  this -> vorbisComm = this -> ddata -> getVorbisComment();

                  std::string query = "INSERT INTO collection VALUES (NULL, \"";
                  query += fullPathFileName_;
                  query += "\", \"";
                  query += this -> replace ( vorbisComm [ "TITLE" ] );
                  query += "\", \"";
                  query += this -> replace ( vorbisComm [ "ARTIST" ] );
                  query += "\", \"";
                  query += this -> replace ( vorbisComm [ "ALBUM" ] );
                  query += "\", \"";
                  query += this -> replace ( vorbisComm [ "TRACKNUMBER" ] );
                  query += "\")";

                  this -> audio -> closeF ( id );
//                   this -> audio -> flush();
                  sql -> exec ( this -> db, query . c_str() );
               }
            }
            free ( namelist [ cnt ] );
         }
         free ( namelist );
      } else {
         perror ( "Couldn't open the directory" );
      }
      chdir ( ".." );
   }
   return 1;
}

/*
   Заменяем все " на ""
   Нужно для SQLite ( ESCAPE )
*/
std::string Scan::replace ( std::string str ) const {
   std::string::size_type pos = 0;
   while ( ( pos = str . find ( "\"", pos ) ) != std::string::npos ) {
      str . erase ( pos, 1 );
      str . insert ( pos, "\"\"" );
      pos += 2;
   }
   return str;
}
