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

#include "CustomQSqlTableModel.hpp"

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace MainWindow {
        namespace Widgets {
          namespace PlayList {
            namespace TrackList {

              CustomQSqlTableModel::CustomQSqlTableModel() :
                trackList(NULL) {
                this -> trackList = TrackList_UI::Instance();
              }

              CustomQSqlTableModel::~CustomQSqlTableModel() {
              }

              QVariant CustomQSqlTableModel::data(const QModelIndex &index, int role) const {
                //                qDebug() << "data";

                QModelIndex currentIndex = this -> trackList -> getSelectedIndex();
                QVariant value = QSqlQueryModel::data(index, role);

                if (value.isValid() && role == Qt::DisplayRole) {
                  if (index.column() == 2) {
                    return value.toString().prepend("#");
                  } else if (index.column() == 2) {
                    return value.toString().toUpper();
                  }
                }

                //                cout  << index.column() << " : " << index.row() << endl;

                if (role == Qt::TextColorRole && index.column() == 3 && currentIndex.row() == index.row()) {
                  this -> trackList -> refresh(index);
//                  cout << index.row() << " : " << currentIndex.row() << endl;
                  return qVariantFromValue(QColor(Qt::blue));
                }

                return value;
              }

//              bool CustomQSqlTableModel::setData(const QModelIndex & index, const QVariant & value, int role) {
//                //                qDebug() << "setata";
//                //                bool lResult = false;
//                //                lResult = QSqlTableModel::setData(index, qVariantFromValue(QColor(Qt::red)), Qt::TextColorRole);
//                //                return lResult;
//              }

            }
          }
        }
      }
    }
  }
}
