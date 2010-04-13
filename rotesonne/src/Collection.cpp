/*******************************************************************************
 * Copyright (c) 2009-2010, Alex Ivasyuv                                       *
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
    status(true), cfg(Configuration::Instance()), player(new Player()), xspf(
        new ::SilentMedia::Media::PlayList::XSPF::XSPF()) {
  }

  Collection::~Collection() {
    delete this -> xspf;
    this -> xspf = NULL;

    delete this -> player;
    this -> player = NULL;
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

  //TODO: add error catch
  void Collection::createDbStructure() {
    const QString CREATE_TABLE =
        "CREATE TABLE collection (id INTEGER PRIMARY KEY AUTOINCREMENT, fileName VARCHAR (255),"
          "tracknum INTEGER, title VARCHAR (255), artist VARCHAR (255), album VARCHAR (255));";

    QSqlQuery q("PRAGMA encoding = 'UTF-8';", this -> db);
    QSqlQuery query(CREATE_TABLE, this -> db);
  }

  // --------------------------------------------------------------------
  // Private methods
  // --------------------------------------------------------------------

  void Collection::run() {
    { // <--- WTF??
      this -> flush();
      this -> scanFiles(this -> scanPath);
      this -> prepareQuery(this -> fileList);
      this -> updateDb();
      this -> updatePlayList();
    }
  }

  //TODO: reset id
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
        string currentPathExt = boost::filesystem::path(itr->filename()).extension();

        if (is_directory(itr->status())) {
          scanFiles(itr->path());
          // TODO: Should be fetch extensions fom file
        } else if (!currentPathExt.compare(".wav") || !currentPathExt.compare(".ogg") || !currentPathExt.compare(
            ".flac") || !currentPathExt.compare(".wv")) {

          this -> fileList.push_back(itr->path().string());
        }
      }
    }
    return true;
  }

  //TODO: rewrite all with QString instead of sstring
  bool Collection::prepareQuery(vector <string> & inputList) {
    // open file, fetch vorbis comments
    for (uint i = 0; i < inputList.size(); ++i) {
      if (status) {

        this -> process = 100 / (static_cast <double> (inputList.size()) / (i + 1));

        string fileId = inputList[i];
        string fileName = inputList[i];

        // populate list for playlist
        this -> filePath << fileName.c_str();

        this -> player -> open(fileName, fileId);
        map <string, string> vorbisComments = this -> player -> getVorbisComments(fileId);

        QString replacedFileName = this -> replace(fileName);
        QString replacedTrackNum = this -> replace(vorbisComments["TRACKNUMBER"]);
        QString replacedTitle;

        if (!this -> replace(vorbisComments["TITLE"]).isEmpty()) {
          replacedTitle = this -> replace(vorbisComments["TITLE"]);
        } else {
          replacedTitle = this -> replace(Path(inputList[i]).filename());
        }

        QString replacedArtist = this -> replace(vorbisComments["ARTIST"]);
        QString replacedAlbum = this -> replace(vorbisComments["ALBUM"]);

        QString query = QString("INSERT INTO collection VALUES (NULL, \"%1\", \"%2\", \"%3\", \"%4\", \"%5\")").arg(
            replacedFileName, replacedTrackNum, replacedTitle, replacedArtist, replacedAlbum);

        this -> player -> close(fileId);

        this -> queryList.push_back(query.toStdString().c_str());
      }
    }
    return true;
  }

  void Collection::updateDb() {
    // create query variable
    string query;

    this -> db.transaction();

    for (uint i = 0; i < this -> queryList.size(); ++i) {
      // execute SQL queries
      this -> db.exec(queryList[i].c_str());
    }

    this -> db.commit();
    this -> db.exec("VACUUM"); // not working?

    this -> status = false; // end
  }

  void Collection::updatePlayList() {
    const QString playListFolderPath = this -> cfg -> getPlayListFolderPath();

    const QString prefix = "file://";
    const QString defaultPlayList = DEFAULT_PLAYLIST + PLAYLIST_EXT;
    const string playList = (playListFolderPath + "/" + defaultPlayList).toStdString();

    list <TrackInfo> playListData;
    //    vector <TrackInfo> trackInfoData;

    for (int i = 0; i < this -> filePath.size(); ++i) {
      TrackInfo trackInfo;

      //      const string artist = "Rammstein";
      //      const string album = "Mutter";
      //      const string title = "Ich Will";
      //      const int trackNum = 4;

      QString trackLocation = this -> filePath[i];
      QString escapeTraclLocation = (QString) ::SilentMedia::Utils::String::toXML(trackLocation.toStdString()).c_str();
      const QString finalTrackLocation = prefix + escapeTraclLocation;

      //            trackInfo1.setArtist(artist);
      //            trackInfo1.setAlbum(album);
      //            trackInfo1.setTitle(title);
      //            trackInfo1.setTrackNumber(trackNum);
      trackInfo.setTrackLocation(finalTrackLocation.toStdString());

      playListData.push_back(trackInfo);
    }
    this -> xspf -> writePlayList(playList, playListData);
  }

  void Collection::showError(QSqlQuery *q) {
    cerr << "Error: Unable to execute sql query" << endl;
    qDebug() << q -> lastError();
  }

  /*
   * Replace all " on ""
   * SQLite ESCAPE
   */
  QString Collection::replace(string str) const {
    string::size_type pos = 0;
    while ((pos = str.find("\"", pos)) != string::npos) {
      str.erase(pos, 1);
      str.insert(pos, "\"\"");
      pos += 2;
    }
    return (QString) str.c_str();
  }

}
