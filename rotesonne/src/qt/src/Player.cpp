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

#include "Player.hpp"

namespace RoteSonne {
  Player::Player() {
    this -> playerStatus = Stop;
  }

  Player::~Player() {
  }

  void Player::setAudioDriver(const string soundDriver, const string driver) {
    init(soundDriver, driver);
  }

  // Player status
  Player::PlayerStatus Player::getPlayerStatus() {
    return this -> playerStatus;
  }

  void Player::setPlayerStatus(PlayerStatus status) {
    this -> playerStatus = status;
  }

  // Info
  string Player::getFileSizeString(const string &fileId) {
    double fileSize = ((getFileSize(fileId)) / 1024 / 1024.00);
    ostringstream out;
    out.precision(3);
    out << fileSize << "Mb";
    return out.str();
  }

  string Player::getSampleRateInfoString(const string &fileId) {
    ostringstream out;
    out << getSampleRate(fileId);
    out << "Hz";
    return out.str();
  }

  string Player::getBitRateString(const string &fileId) {
    ostringstream out;
    out << getBitRate(fileId) / 1000;
    out << "kbps";
    return out.str();
  }

  string Player::getTotalTime(const string &fileId) {
    ostringstream out;
    out
        << (static_cast < int > (Audio::getTotalTime(fileId))
            / 60); // min
    out << ":";
    out
        << (static_cast < int > (Audio::getTotalTime(fileId))
            % 60); // sec

    return out.str();
  }

}
