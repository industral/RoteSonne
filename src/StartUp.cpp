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

#include "StartUp.hpp"

namespace RoteSonne {

  // --------------------------------------------------------------------
  // Public methods
  // --------------------------------------------------------------------

  StartUP::StartUP() :
    cfg(Configuration::Instance()) {
  }

  StartUP::~StartUP() {
  }

  bool StartUP::init() {
    this -> checkApplicationDir(cfg -> getHomeDirApplication());
    this -> checkApplicationDir(cfg -> getPlayListFolderPath());
    return true; // TODO: fix
  }

  // --------------------------------------------------------------------
  // Private methods
  // --------------------------------------------------------------------

  //TODO: Rewrite with throws
  bool StartUP::checkApplicationDir(const QString &path) {
    if (this -> qDir.exists(path)) {
      return true;
    } else {
      if (this -> qDir.mkdir(path)) {
        return true;
      } else {
        return false;
      }
    }
    return false;
  }

}
