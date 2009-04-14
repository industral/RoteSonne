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

#ifndef APP_H
#define APP_H

#include <QtGui>

#include <SML/all.hpp>
// #include <libsscc/libsscc.hpp>

#include "ssnnmc.h"
#include "player.h"

class SSMix;
class Player;
class Scan;

class App : public QMainWindow {
   Q_OBJECT

   public:
      App ( );
      ~App ( );
      
      void setTitle ( std::string str );
   private:
      void createActions ( void );
      void createMenus ( void );

      SSMix * ssmix;
      Player * player;
      SilentMedia::SoundSystem::OSS::Mixer * ossmix;

//       QPushButton * pb; //test
      
      //BEGIN ACTION ELEMENTS
      QAction * newAct;
      QAction * exitAct;
      QAction * rescanAct;
      QAction * preferents;
      QAction * aboutAct;
      QAction * aboutOSSAct;
      QAction * aboutQtAct;
//       QAction * loadCoverAct;

      QMenu * fileMenu;
      QMenu * editMenu;
      QMenu * formatMenu;
      QMenu * helpMenu;
      //END
   private slots:
      void aboutQt ( void );
      void aboutOSS ( void );
      void aboutQSSMix ( void );
//       void loadCover ( void );
   protected:
//       void contextMenuEvent ( QContextMenuEvent * event );
};

#endif
