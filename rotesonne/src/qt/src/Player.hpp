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

#ifndef _ROTESONNE_PLAYER_HPP_
#define _ROTESONNE_PLAYER_HPP_

#include <libsml/all.hpp>
#include <libsml/Audio/Audio.hpp>

namespace RoteSonne {
  class Player {
    public:

      enum PlayerStatus {
        Stop, Play, Pause
      };

      /**
       * Default constructor.
       */
      Player();

      /**
       * Default destructor.
       */
      ~Player();

      void setAudioDriver(const string &driver);
      void setAudioDriver();
      bool open(const string &fileName, const string &fileId);
      void close(const string &fileId);
      void play(const string &fileId, bool resume = false);
      void pause(const string &fileId);
      void stop(const string &fileId);

      // seek
      void setSeek(const string &fileId, const float &seek);
      float getSeek(const string &fileId);

      PlayerStatus getPlayerStatus();
      void setPlayerStatus(PlayerStatus status);

      // Info
      string getFileSizeString(const string &fileId);
      int getChannelInfo(const string &fileId);
      int getBitsPerSample(const string &fileId);
      string getSampleRateInfoString(const string &fileId);
      string getBitRateString(const string &fileId);
      string getTotalTime(const string &fileId);
    private:
      // SilentMedia audio object
      SilentMedia::Audio::Audio * audio;

      PlayerStatus playerStatus;
  };
}

#endif
