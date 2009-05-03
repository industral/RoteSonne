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

#include "Collection.hpp"

namespace RoteSonne {

  /**
   * DB structure:
   PRAGMA encoding = "UTF-8";
   CREATE TABLE collection (id INTEGER PRIMARY KEY AUTOINCREMENT, fileName VARCHAR (255),
   tracknum INTEGER, title VARCHAR (255), artist VARCHAR (255), album VARCHAR (255));

   */

  // --------------------------------------------------------------------
  // Public methods
  // --------------------------------------------------------------------

  Collection::Collection() :
    status(true) {
    this -> player = new Player();
  }

  Collection::~Collection() {
  }

  void Collection::open() {
    // connect to existing connection
    this -> db = QSqlDatabase::database();
  }

  void Collection::startScan(const string &path) {
    this -> scanPath = path;
    this -> start();
  }

  void Collection::stopScan() {
    this -> status = false;
  }

  long Collection::getProcess() const {
    return this -> process;
  }

  bool Collection::getStatus() const {
    return status;
  }

  // --------------------------------------------------------------------
  // Private methods
  // --------------------------------------------------------------------

  void Collection::run() {
    this -> flush();
    this -> scanFiles(this -> scanPath);
    this -> updateDb();
  }

  void Collection::flush() {
    this -> db.exec("DELETE FROM collection");
  }

  bool Collection::scanFiles(const boost::filesystem::path &path) {
    if (!exists(path)) {
      return false;
    }

    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr) {
      if (status) {
        string currentPathExt =
            boost::filesystem::path(itr->filename()).extension();

        if (is_directory(itr->status())) {
          scanFiles(itr->path());
          // TODO: Should be fetch extensions fom file
        } else if (!currentPathExt.compare(".wav") || !currentPathExt.compare(
            ".ogg") || !currentPathExt.compare(".flac")
            || !currentPathExt.compare(".wv")) {

          // open file, fetch vorbis comments
          string fileId = itr->path().string();
          string fileName = itr->path().string();

          this -> player -> open(fileName, fileId);
          map < string, string > vorbisComments =
              this -> player -> getVorbisComments(fileId);

          stringstream out;
          out << "INSERT INTO collection VALUES (NULL, '";
          out << this -> replace(itr->path().string());
          out << "', '";

          out << this -> replace(vorbisComments["TRACKNUMBER"]);
          out << "', '";

          if (!this -> replace(vorbisComments["TITLE"]).empty()) {
            out << this -> replace(vorbisComments["TITLE"]);
          } else {
            out << this -> replace(itr->path().filename());
          }
          out << "', '";

          out << this -> replace(vorbisComments["ARTIST"]);
          out << "', '";

          out << this -> replace(vorbisComments["ALBUM"]);
          out << "');";

          this -> player -> close(fileId);

          this -> queryList.push_back(out.str());
        }
      }
    }
    return true;
  }

  void Collection::updateDb() {
    // create query variable
    string query;

    this -> db.transaction();

    for (uint i = 0; i < this -> queryList.size(); ++i) {

      this -> process = 100
          / (static_cast < double > (this -> queryList.size()) / (i + 1));

      // execute SQL queries
      this -> db.exec(queryList[i].c_str());

    }

    this -> db.commit();
    this -> db.exec("VACUUM"); // not working?

    this -> status = false; // end
  }

  void Collection::showError(QSqlQuery *q) {
    cerr << "Error: Unable to execute sql query" << endl;
    qDebug() << q -> lastError();
  }

  /*
   * Replace all " on ""
   * SQLite ESCAPE
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

}
