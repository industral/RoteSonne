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

  // --------------------------------------------------------------------
  // Public methods
  // --------------------------------------------------------------------

  Collection::Collection() :
    status(true) {
  }

  Collection::~Collection() {
  }

  void Collection::open(const string &dbName) {
    // connect to existing connection
    this -> db = QSqlDatabase::database();
  }

  void Collection::scan(const string &path) {
    this -> scanPath = path;
    this -> start();
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

      string currentPathExt =
          boost::filesystem::path(itr->filename()).extension();

      if (is_directory(itr->status())) {
        scanFiles(itr->path());
        // TODO: Should be fetch extensions fom file
      } else if (!currentPathExt.compare(".wav") || !currentPathExt.compare(
          ".ogg") || !currentPathExt.compare(".flac")
          || !currentPathExt.compare(".wv")) {
        string query = "INSERT INTO collection VALUES (NULL, \"";
        query += this -> replace(itr->path().string());
        query += "\", \"";
        query += "" /*this -> replace(vorbisComm["TITLE"])*/;
        query += "\", \"";
        query += "" /*this -> replace(vorbisComm["ARTIST"])*/;
        query += "\", \"";
        query += "" /*this -> replace(vorbisComm["ALBUM"])*/;
        query += "\", \"";
        query += "" /*this -> replace(vorbisComm["TRACKNUMBER"])*/;
        query += "\");";

        this -> queryList.push_back(query);
      }
    }
    return true;
  }

  void Collection::updateDb() {
    // create query variable
    string query;

    this -> db.transaction();

    for (int i = 0; i < this -> queryList.size(); ++i) {
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
