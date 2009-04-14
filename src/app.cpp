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

#include "app.h"

App::App ( ) : ssmix ( new SSMix ), player ( new Player ),
            ossmix ( SilentMedia::SoundSystem::OSS::Mixer::Instance() ) {
   // Выставляем кодировку по умолчанию UTF-8
   QTextCodec *codec = QTextCodec::codecForName ( "UTF-8" );
   QTextCodec::setCodecForTr ( codec );
   QTextCodec::setCodecForLocale ( codec );
   QTextCodec::setCodecForCStrings ( codec );

   this -> ssmix -> init();
   this -> player -> init();

   QTabWidget * mainTabWidget = new QTabWidget;
   mainTabWidget -> addTab ( player -> retW(), QString ( "Player" ) );
   mainTabWidget -> addTab ( ssmix -> retW(), QString ( "Mixer" ) );

   QVBoxLayout * Layout = new QVBoxLayout;
   Layout -> addWidget ( mainTabWidget );
//       //test
//    pb = new QPushButton ( "OK:)" );
//    Layout -> addWidget ( pb );

   QWidget * widget = new QWidget;
   widget -> setLayout ( Layout );
   


   setCentralWidget ( widget );
   setWindowIcon ( QIcon ( ":/img/qssmix.png" ) );
   setWindowTitle ( tr ( "SSNGMC" ) );
   resize ( 1200, 760 );
   setMinimumSize ( 320, 240 );

   QFont font ( "Verdana", 10 );
   setFont ( font );

   this -> createActions();
   this -> createMenus();
}

App::~App ( ) {
   this -> player -> finish();
}

// void App::contextMenuEvent ( QContextMenuEvent * event ) {
//    QMenu menu ( this );
//    menu.addAction ( loadCoverAct );
// //    menu.addAction ( copyAct );
// //    menu.addAction ( pasteAct );
// //    menu.exec ( event->globalPos() );
// //    menu . exec ( pb->mapToGlobal(QPoint(0, 0)) );
// }

// void App::loadCover ( void ) {
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
// }

void App::setTitle ( std::string str ) {
   setWindowTitle ( str . c_str() );
}

void App::createActions ( void ) {
   newAct = new QAction ( tr ( "&Save config" ), this );
   newAct -> setShortcut ( tr ( "Ctrl+S" ) );
   newAct -> setStatusTip ( tr ( "Save current mixer configuration" ) );
   connect ( newAct, SIGNAL ( triggered ( ) ), this, SLOT ( saveConfig ( ) ) );

   rescanAct = new QAction ( tr ( "&Rescan controls" ), this );
   rescanAct -> setShortcut ( tr ( "Ctrl+R" ) );
   rescanAct -> setStatusTip ( tr ( "rescan control devices" ) );
   connect ( rescanAct, SIGNAL ( triggered ( ) ), this, SLOT ( rescanCtrlDev ( ) ) );

   exitAct = new QAction ( tr ( "E&xit" ), this );
   exitAct -> setShortcut ( tr ( "Ctrl+Q" ) );
   exitAct -> setStatusTip ( tr ( "Exit the application" ) );
   connect ( exitAct, SIGNAL ( triggered ( ) ), this, SLOT ( close ( ) ) );

   preferents = new QAction ( tr ( "&Preferents" ), this );
//    preferents -> setShortcut ( tr ( "Ctrl+P" ) );
//    undoAct->setStatusTip ( tr ( "Undo the last operation" ) );
   connect ( preferents, SIGNAL ( triggered() ), this -> player, SLOT ( PreferentsWindow() ) );

   aboutAct = new QAction ( tr ( "&About" ), this );
   aboutAct -> setIcon ( QIcon ( ":/img/qssmix.png" ) );
   aboutAct -> setStatusTip ( tr ( "Show information aboutQSSMix QSSMix" ) );
   connect ( aboutAct, SIGNAL ( triggered ( ) ), this, SLOT ( aboutQSSMix ( ) ) );

   aboutOSSAct = new QAction ( tr ( "About OSS" ), this );
//   aboutOSSAct -> setIcon ( QIcon ( ":/img/oss-small.png" ) );
   aboutOSSAct -> setStatusTip ( tr ( "Show information aboutQSSMix OSS" ) );
   connect ( aboutOSSAct, SIGNAL ( triggered() ), this, SLOT ( aboutOSS ( ) ) );

   aboutQtAct = new QAction ( tr ( "About &Qt" ), this );
   aboutQtAct -> setIcon ( QIcon ( ":/img/qt.png" ) );
   aboutQtAct->setStatusTip ( tr ( "Show the Qt library's About box" ) );
   connect ( aboutQtAct, SIGNAL ( triggered ( ) ), qApp, SLOT ( aboutQt ( ) ) );
   connect ( aboutQtAct, SIGNAL ( triggered ( ) ), this, SLOT ( aboutQt ( ) ) );
   
//    loadCoverAct = new QAction ( tr ( "Load Cover" ), this );
//    loadCoverAct -> setShortcut ( tr ( "Ctrl+O" ) );
//    loadCoverAct -> setStatusTip ( tr ( "Load new Cover" ) );
//    connect ( loadCoverAct, SIGNAL ( triggered ( ) ), this, SLOT ( loadCover ( ) ) );

}

void App::createMenus ( void ) {
   fileMenu = menuBar()->addMenu ( tr ( "&File" ) );
   fileMenu->addAction ( newAct );
   fileMenu -> addAction ( rescanAct );

   fileMenu->addSeparator();
   fileMenu->addAction ( exitAct );

   editMenu = menuBar() -> addMenu ( tr ( "&Preferents" ) );
   editMenu->addAction ( preferents );
//    editMenu->addSeparator();

   helpMenu = menuBar() -> addMenu ( tr ( "&Help" ) );
   helpMenu -> addAction ( aboutAct );
   helpMenu -> addAction ( aboutOSSAct );
   helpMenu->addAction ( aboutQtAct );
}

void App::aboutQt ( void ) { }

void App::aboutOSS ( ) {
   QMessageBox msgBox;
   msgBox . setIconPixmap ( QPixmap ( ":/img/oss.png" ) );
   msgBox . addButton ( QMessageBox::Ok );
   msgBox . setWindowTitle ( tr ( "Об Open Sound System ( OSS )" ) );
   msgBox . setText ( tr (
                      "The <b> Open Sound System </b> ( OSS ) is a standard interface for making and capturing sound in Unix operating systems developping by <b> 4Front Technologies </b>."
         "<p><a href = 'http://www.opensound.com'> http://www.opensound.com </a>"
         "<p><a href = 'http://www.opensound.com/oss.html'> http://www.opensound.com/oss.html </a>"
         "<p><a href = 'http://developer.opensound.com'> http://developer.opensound.com </a>" ) );
   msgBox . exec ( );
}

void App::aboutQSSMix ( void ) {
   QMessageBox msgBox;
   msgBox . setIconPixmap ( QPixmap ( ":/img/qssmix.png" ) );
   msgBox . addButton ( QMessageBox::Ok );
   msgBox . setWindowTitle ( tr ( "О программе QSSMix" ) );
   msgBox . setText ( tr (
                      "QSSMix - профессиональный кросс-платформенный микшер для звуковой системы Open Sound System ( OSS )"
         "<p> ( Qt SIEGERSTEIN Mixer )"
         "<p> Qt: v%1"
         "<p> libssoss: v%2"
         "<p> libssoss API: v%3"
                         ) . arg ( QT_VERSION_STR )/* . arg ( ossmix -> getSSOSSVersion().c_str() )*/
         /*arg ( ossmix -> getSSOSSAPIVersion().c_str() )*/ );
   msgBox . exec ( );
}

