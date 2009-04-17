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

/*
 ВНИМАНИЕ:
 Используем stat(2) а не d_type из dirent.h, потому что этот параметр может не работать
 на всех файловых системах, в частности это касается raiserfs.
 */

#include "Collection.hpp"

static int filter(const struct dirent * dir) {
  if ((dir -> d_name[0] == '.') && (dir -> d_name[1] == 0)) {
    return 0;
  }
  if ((dir -> d_name[0] == '.') && (dir -> d_name[1] == '.')
      && (dir -> d_name[2] == 0)) {
    return 0;
  }
  return 1;
}

Collection::Collection() {
}

Collection::~Collection() {
}

void Collection::open() {
  if (sqlite3_open("collection.db", &this -> db)) {
    cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    this -> close();
//    exit(1);
  }
}

void Collection::close() {
  sqlite3_close(this -> db);
}

void Collection::flush() {
  sqlite3_exec(this -> db, "DROP TABLE collection", NULL, NULL, NULL);
  sqlite3_exec(this -> db,
      "CREATE TABLE collection (id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "fileName VARCHAR (255), title VARCHAR (255), artist VARCHAR (255),"
        "album VARCHAR (255), tracknum INTEGER )", NULL, NULL, NULL);
}

int Collection::scan(string path, int level) {
  if (!path.empty()) {

    if (chdir(path.c_str()) == -1) {
      //  if (errno == )
      //      cout << "Error to chdir() in Collection::Collection() " << endl;
    }

    struct dirent **namelist;
    struct stat statBuf;

    int n = scandir("./", &namelist, filter, alphasort);

    if (n >= 0) {
      int cnt;
      for (cnt = 0; cnt < n; ++cnt) {

        char currPathBuf[255];
        if (getcwd(currPathBuf, 254) == NULL) {
          perror("getcwd error");
        }

        string fullPathFileName_;
        string fullPathFileName = currPathBuf;

        fullPathFileName += "/";
        fullPathFileName_ = fullPathFileName;
        fullPathFileName += namelist[cnt] -> d_name;

        if (stat(fullPathFileName.c_str(), &statBuf) != 0) {
          cerr << "Error in stat() for file " << fullPathFileName << endl;
        }

        if (statBuf.st_mode & S_IFDIR) { // if dir
          scan(namelist[cnt] -> d_name, level + 1);
        }

        if (statBuf.st_mode & S_IFREG) { // if file
          string fileName = namelist[cnt] -> d_name;
          string ext = fileName.substr(
              ((fileName.find(".", fileName.size() - 5)) + 1), fileName.size());
          if (ext == "wav" || ext == "ogg" || ext == "flac" || ext == "wv") {

            fullPathFileName_ += this -> replace(fileName);

            string id = fullPathFileName;
            //            this -> audio -> openFile(fullPathFileName, id);
            //            this -> vorbisComm = this -> ddata -> getVorbisComment();

            std::string query = "INSERT INTO collection VALUES (NULL, \"";
            query += fullPathFileName_;
            query += "\", \"";
            query += "" /*this -> replace(vorbisComm["TITLE"])*/;
            query += "\", \"";
            query += "" /*this -> replace(vorbisComm["ARTIST"])*/;
            query += "\", \"";
            query += "" /*this -> replace(vorbisComm["ALBUM"])*/;
            query += "\", \"";
            query += "" /*this -> replace(vorbisComm["TRACKNUMBER"])*/;
            query += "\")";

            //            this -> audio -> closeF(id);
            //                   this -> audio -> flush();
            sqlite3_exec(this -> db, query.c_str(), NULL, NULL, NULL);
          }
        }
        //        free(namelist[cnt]);
      }
      //      free(namelist);
    } else {
      perror("Couldn't open the directory");
    }
    chdir("..");
  }
  return 1;
}

/*
 Заменяем все " на ""
 Нужно для SQLite ( ESCAPE )
 */
string Collection::replace(string str) const {
  string::size_type pos = 0;
  while ((pos = str.find("\"", pos)) != string::npos) {
    str.erase(pos, 1);
    str.insert(pos, "\"\"");
    pos += 2;
  }
  return str;
}
