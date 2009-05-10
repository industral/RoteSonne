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

#ifndef _ROTESONNE_COLLECTOPN_HPP_
#define _ROTESONNE_COLLECTOPN_HPP_

#include <iostream>
#include <string>
#include <map>
#include <vector>

// boost filesystem
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

// include DB
#include <QtSql>

// include thread
#include <QThread>

// include Player
#include "Player.hpp"

typedef boost::filesystem::path Path;

using namespace std;

namespace RoteSonne {
  class Collection: public QThread {
    public:
      Collection();
      ~Collection();

      void open();
      void startScan(const string &path);
      void stopScan();
      long getProcess() const;
      bool getStatus() const;

    private:
      virtual void run();

      void flush();
      bool scanFiles(const boost::filesystem::path &path);
      bool createQuery();
      string replace(string str) const;
      void updateDb();
      void showError(QSqlQuery *q);

      // variables
      QSqlDatabase db;
      Player * player;

      map < string, string > vorbisComm;
      vector < string > queryList;
      vector < string > fileList;

      long process;
      bool status;
      string scanPath;
  };
}

#endif
