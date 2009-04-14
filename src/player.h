/***************************************************************************
 *   Copyright (C) 2008 by Alex J. Ivasyuv                                 *
 *   alex@siegerstein.org.ua                                               *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui>
#include <QtSql>

#include "app.h"
#include "scanc.h"

class Scan;
class SQLite;

class Player : public QMainWindow {
   Q_OBJECT
   public:
      Player ( );
      void finish ( void );
      void init ( void );
      QWidget * retW();
   private:
      // functions
      void closeDbConn ( void );
      void initDbConn ( void );
      void playListSetup ( void );
      void setSeek ( int val );
      void setInfo ( void );
      void setCoverSize ( const QPixmap * pix ); // установка размера обложки
      
      // variables
      SilentMedia::Audio * audio;
      SilentMedia::DecodedData * ddata;
      
//       libssoss::DSP * dsp;
//       App * app;
      Scan * scan;
      QSqlDatabase * collectionDb;
      SQLite * settingsDbConn;
      QTimer * timer;
      
      // param var
      QSize defaultSize;
      double imageSize; // размер обложки ( напр. 250 x 250 )
   
      //BEGIN control buttons
      QToolButton * buttonPrev;
      QToolButton * buttonPlay;
      QToolButton * buttonPause;
      QToolButton * buttonStop;
      QToolButton * buttonForward;
      //END

      QGridLayout * metaInfoLayout;
      QMap < std::string, QLabel * > ql_VorbisComment_label;
      QMap < std::string, QLineEdit * > ql_VorbisComment_lineEdit;
      QMap < std::string, QSpinBox * > ql_VorbisComment_spinBox;
      QToolButton * metaInfoEditButton;
      QGroupBox * trackTechInfo;
      
      // виджеты для технической информации
//       QLabel * ql_fileName;
      QLabel * ql_sampleRate;
      QLabel * ql_channels;
      QLabel * ql_bitPerSample;
      QLabel * ql_bitRate;
      QLabel * ql_totalTime;
      QLabel * ql_fileSize;

      QWidget * playerWidget;
      QVBoxLayout * il;
      QVBoxLayout * ql;
      QHBoxLayout * qlh;
      QVBoxLayout * qlv;
      QHBoxLayout * qlm;
   
      QQueue < QLayoutItem * > qlQueue;
      // набор обычных елементов
      QQueue < QWidget * > qlhQueue;
      QQueue < QWidget * > qlvQueue;
      // набор елементов которые должны быть или disable() или enable()
      QQueue < QWidget * > qlhQueueEdit;
   
      QLabel * imageLabel;
      QAction * loadCoverAct;
   
      QSlider * posSlider;
      QTableView * view;
      QSqlTableModel * model;
      int maxRow;
   
      QHBoxLayout * LLayout;
      QVBoxLayout * RLayout;
      QHBoxLayout * MLayout;
      
      int sliderPos;
   
      // стражи
      bool setInfoCheck;
      bool editMetaInfoCheck;
      bool pauseCheck;
      
      SilentMedia::Status globalSongStatus;
      
      QWidget * preferentWidget;
      QFileDialog * fileDialog;
      std::string collectionFolder;
      QLineEdit * collectionLineEdit;

      // табы
      QTabWidget * playerTabWidget;
      
      std::string currFileName;
      QModelIndex index;
      QModelIndex playIndex;
      
      std::string id; // глобальный id
      std::string tmpId; // id от initLocation
      std::string prevId; // предыдущий id от initLocation
   private slots:
      void Play ( void );
      void Pause ( void );
      void Stop ( void );
      void Previous ( void );
      void Forward ( void );
      void initLocation ( const QModelIndex & index );
      void updateSliderPos ( void );
      void rescanDb ( void );

      void disconnectSlider ( void );
      void connectSlider ( void );
      void writeSliderPos ( int val );
      void editMETAComment ( void );

      void showCoverMenu ( const QPoint & p ); // контекстное меню для обложки
      void loadCover ( void ); // SLOT для обложки
      
      void PreferentsWindow ( void );
      void openFileDialogFolder ( void );
      // button
      void ApplySlot ( void );
      void OKSlot ( void );
      
      void ReleaseMem ( void );
};

#endif
