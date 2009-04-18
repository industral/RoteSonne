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

#include <unistd.h> // chdir(2), stat(2)
#include <sys/types.h> // stat(2)
#include <sys/stat.h> // stat(2)
#include <cstdlib>

#include <dirent.h>
#include <sqlite3.h>

using namespace std;

namespace RoteSonne {
  class Collection {
    public:
      Collection();
      ~Collection();

      void open();
      void flush();
      void close();
      int scan(string path, int level = 0);
      string replace(string str) const;
    private:
      sqlite3 * db;
      //    SQLite * sql;

      //    SilentMedia::Audio * audio;
      //    SilentMedia::DecodedData * ddata;
      //       libssoss::DSP * dsp;
      //       libssoss::DSP * thdsp;
      map < string, string > vorbisComm;
  };
}

#endif
