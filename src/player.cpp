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

/*
   Основной класс плеера. Будет отображаться в виде таба в основной программе ( app ), через retW()
*/


#include "player.h"

Player::Player ( void ) :
      audio ( SilentMedia::Audio::Instance() ), ddata ( SilentMedia::DecodedData::Instance() ), scan ( new Scan ), collectionDb ( new QSqlDatabase ), settingsDbConn ( new SQLite )
      , timer ( new QTimer ), defaultSize ( 32, 32 ), imageSize ( 250 ), setInfoCheck ( 0 ), editMetaInfoCheck ( 0 ), pauseCheck ( false )
      , globalSongStatus ( SilentMedia::U ), maxRow ( -1 )
{ }

void Player::finish ( void ) {

//    audio -> changeStatus ( SilentMedia::Stop );
//    delete app;
   delete audio;
//    delete scan;
//    delete collectionDb;
//    delete timer;
   
   this -> closeDbConn ( );
}

void Player::init ( void ) {
//    std::cout << "DSP_PL: " << this -> audio << std::endl;
//    audio -> init ( _DSP, _OSS, "/dev/dsp" );
   audio -> init ( _Game, _DSP, _OSS, "/dev/dsp" );
//    audio -> init ( _Game, _DSP, _ALSA, "default" );
//    this -> audio -> init ( "/dev/audio" );
//    this -> audio -> makeconnect ( this -> audio, "PLAYER" );
//    this -> audio -> setDSP ( this -> audio );

   // инициализация соединения для таблици collection.db
   *this -> collectionDb = QSqlDatabase::addDatabase ( "QSQLITE" );

   // инициализация соединения для таблици settings.db
   this -> settingsDbConn -> initVal();

   scan -> init ( SQLITE );
   this -> view = new QTableView;
   this -> playListSetup();

   this -> LLayout = new QHBoxLayout;
   this -> RLayout = new QVBoxLayout;
   this -> MLayout = new QHBoxLayout;

   QToolButton * scanDbButton = new QToolButton ( this );
   scanDbButton -> setIcon ( QIcon ( ":/img/player/rebuild.png" ) );
   scanDbButton -> setAutoRaise ( true );

   //BEGIN Кнопки для воспроизведения
   buttonPrev = new QToolButton ( this );
   buttonPrev -> setIcon ( QIcon ( ":/img/player/player_rew.png" ) );
   buttonPrev -> setIconSize ( defaultSize );
   buttonPrev -> setAutoRaise ( true );
   
   buttonPlay = new QToolButton ( this );
   buttonPlay -> setIcon ( QIcon ( ":/img/player/player_play.png" ) );
   buttonPlay -> setIconSize ( defaultSize );
   buttonPlay -> setAutoRaise ( true );
   
   buttonPause = new QToolButton ( this );
   buttonPause -> setIcon ( QIcon ( ":/img/player/player_pause.png" ) );
   buttonPause -> setIconSize ( defaultSize );
   buttonPause -> setAutoRaise ( true );
   
   buttonStop = new QToolButton ( this );
   buttonStop -> setIcon ( QIcon ( ":/img/player/player_stop.png" ) );
   buttonStop -> setIconSize ( defaultSize );
   buttonStop -> setAutoRaise ( true );
   
   buttonForward = new QToolButton ( this );
   buttonForward -> setIcon ( QIcon ( ":/img/player/player_fwd.png" ) );
   buttonForward -> setIconSize ( defaultSize );
   buttonForward -> setAutoRaise ( true );
   //END

   //BEGIN SLIDER
   posSlider = new QSlider ( this );
   posSlider -> setOrientation ( Qt::Horizontal );
   // при движении ползунка музыка идет нормально, пока не отпустить ползунок
   // при движение ползунка записываем положение
   connect ( posSlider, SIGNAL ( valueChanged ( int ) ), this, SLOT ( writeSliderPos ( int ) ) );
   // при перемещение ползунка отключаем обновление
   connect ( posSlider, SIGNAL ( sliderPressed() ), this, SLOT ( disconnectSlider() ) );
   // при отпускании ползунка ставим композицию в выбраное положение, и включаем обновление
   connect ( posSlider, SIGNAL ( sliderReleased() ), this, SLOT ( connectSlider() ) );

   // Обновление позиции ползунка каждые n секунд
   connect ( timer, SIGNAL ( timeout() ), this, SLOT ( updateSliderPos() ) );
   timer -> start ( 50 );
   //END
   
   //BEGIN SLOTS for Play, Pause, Stop
   connect ( buttonPlay, SIGNAL ( clicked() ), this, SLOT ( Play() ) );
   connect ( buttonPause, SIGNAL ( clicked() ), this, SLOT ( Pause() ) );
   connect ( buttonStop, SIGNAL ( clicked() ), this, SLOT ( Stop() ) );
   connect ( buttonPrev, SIGNAL ( clicked() ), this, SLOT ( Previous() ) );
   connect ( buttonForward, SIGNAL ( clicked() ), this, SLOT ( Forward() ) );
   
//    exitAct = new QAction ( tr ( "E&xit" ), this );
//    new QShortcut(QKeySequence("Ctrl+C"), this, SLOT(copy()), 0, Qt::WidgetShortcut);
//    buttonPlay -> setShortcut ( "XF86AudioPlay" );
//    exitAct -> setStatusTip ( tr ( "Exit the application" ) );
//    connect ( exitAct, SIGNAL ( triggered ( ) ), this, SLOT ( close ( ) ) );

   //END

   QHBoxLayout * buttonLayout = new QHBoxLayout;
   buttonLayout -> setAlignment ( Qt::AlignLeft );
   buttonLayout -> addWidget ( scanDbButton );

   connect ( scanDbButton, SIGNAL ( clicked() ), this, SLOT ( rescanDb() ) );

   QHBoxLayout * playerLayout = new QHBoxLayout;
   playerLayout -> setAlignment ( Qt::AlignLeft );
   playerLayout -> addWidget ( buttonPrev );
   playerLayout -> addWidget ( buttonPlay );
   playerLayout -> addWidget ( buttonPause );
   playerLayout -> addWidget ( buttonStop );
   playerLayout -> addWidget ( buttonForward );
   playerLayout -> addWidget ( posSlider );

   QWidget * buttonUPWidget = new QWidget;
   buttonUPWidget -> setLayout ( buttonLayout );

   QWidget * buttonWidget = new QWidget;
   buttonWidget -> setLayout ( playerLayout );
   
   //BEGIN ACTION
   loadCoverAct = new QAction ( tr ( "Load Cover" ), this );
//    loadCoverAct -> setStatusTip ( tr ( "Load new Cover" ) );
   connect ( loadCoverAct, SIGNAL ( triggered() ), this, SLOT ( loadCover() ) );
   //END
   
   ql = new QVBoxLayout;
   qlm = new QHBoxLayout;
   trackTechInfo = new QGroupBox ( tr ( "Technical information" ) );
   trackTechInfo -> setLayout ( ql );

//    ql_fileName = new QLabel;
   ql_sampleRate = new QLabel;
   ql_channels = new QLabel;
   ql_bitPerSample = new QLabel;
   ql_bitRate = new QLabel;
   ql_totalTime = new QLabel;
   ql_fileSize = new QLabel;

   qlv = new QVBoxLayout;
      /*
   Обязательные параметры, которые есть у каждой композиции,
   поэтому мы их не проверяем, а сразу добавляем
      */
//    qlv -> addWidget ( ql_fileName );
   qlv -> addWidget ( ql_sampleRate );
   qlv -> addWidget ( ql_channels );
   // Не обязательный параметр, поэтому прячем его. Если он будет, то мы его показываем -> show();
   qlv -> addWidget ( ql_bitPerSample );
   ql_bitPerSample -> hide();
   qlv -> addWidget ( ql_bitRate );
   qlv -> addWidget ( ql_totalTime );
   qlv -> addWidget ( ql_fileSize );

   ql -> addLayout ( qlv );
   
   //FIXME: Как установить ширину для QHBoxLayout?
   qlh = new QHBoxLayout;
   
   metaInfoEditButton = new QToolButton ( this );
   metaInfoEditButton -> setIcon ( QIcon ( ":/img/player/edit.png" ) );
   metaInfoEditButton -> setIconSize ( defaultSize );
   metaInfoEditButton -> setAutoRaise ( true );
   connect ( metaInfoEditButton, SIGNAL ( clicked() ), this, SLOT ( editMETAComment() ) );

   imageLabel = new QLabel;
   imageLabel -> setScaledContents ( true );
   imageLabel -> setContextMenuPolicy ( Qt::CustomContextMenu );
   connect ( imageLabel, SIGNAL ( customContextMenuRequested ( const QPoint & ) ), this, SLOT ( showCoverMenu ( const QPoint & ) ) );

   il = new QVBoxLayout;
   il -> addWidget ( imageLabel );

   LLayout -> addLayout ( il );

   RLayout -> addWidget ( buttonUPWidget );
   RLayout -> addWidget ( view );
   RLayout -> addWidget ( buttonWidget );

   MLayout -> addLayout ( LLayout, 1 );
   MLayout -> addLayout ( RLayout, 4 );

   playerWidget = new QWidget;
   playerWidget -> setLayout ( MLayout );

   QWidget * metaInfoWidget = new QWidget;
   metaInfoWidget -> setLayout ( qlm );

   playerTabWidget = new QTabWidget;
   playerTabWidget -> addTab ( playerWidget, "Main" );
   playerTabWidget -> addTab ( metaInfoWidget, "Advansed" );
}

void Player::closeDbConn ( void ) {
   /*
   Нужно чтобы небыло этого гемороя:
   QSqlDatabasePrivate::removeDatabase: connection 'qt_sql_default_connection' is still in use, all queries will cease to work.
   */
   *this -> collectionDb = QSqlDatabase();

   if ( this -> collectionDb -> isOpen() ) {
      this -> collectionDb -> close();
   }
   if ( this -> collectionDb ->isValid() ) {
      this -> collectionDb -> removeDatabase ( this -> collectionDb -> connectionNames()[0] );
   }
}

void Player::initDbConn ( void ) {
   this -> collectionDb -> setDatabaseName ( "collection.db" );
   this -> collectionDb -> open();
}

void Player::playListSetup ( void ) {
   this -> initDbConn();

   model = new QSqlTableModel;
   model -> setTable ( "collection" );
   model -> setEditStrategy ( QSqlTableModel::OnManualSubmit ); // изменения в БД вступают в силу только при явном указании submitAll()
   model -> select();
   model -> setHeaderData ( 2, Qt::Horizontal, tr ( "Name" ) );
   model -> setHeaderData ( 3, Qt::Horizontal, tr ( "Artist" ) );
   model -> setHeaderData ( 4, Qt::Horizontal, tr ( "Album" ) );

   this -> maxRow = this -> model -> rowCount();
   for ( int i = 0; i < model -> rowCount(); ++i ) {
      QSqlRecord record = model -> record ( i );
      QString salary = record . value ( "title" ) . toString();
      if ( salary.isEmpty() ) {
         QModelIndex index;
         index = model -> index ( i, 2 );

         std::string fullPath = record . value ( "fileName" ) . toString() . toStdString();
         size_t found = fullPath . find_last_of ( "/" );
         std::string file = fullPath . substr ( found + 1 );

         model -> setData ( index, file . c_str() );
      }
   }

   view -> setSelectionBehavior ( QAbstractItemView::SelectRows ); //  выбираем строку целиком
   view -> setModel ( model );
   view -> setEditTriggers ( QAbstractItemView::NoEditTriggers ); // запрещаем менять при двойном клике
   view -> setColumnWidth ( 2, 440 );
   view -> setColumnWidth ( 3, 220 );
   view -> setColumnWidth ( 4, 220 );
   
   view -> hideColumn ( 0 );
   view -> hideColumn ( 1 );
   view -> hideColumn ( 5 );

   //FIXME: как сделать чтобы при doubleClicked не работал  pressed?
   connect ( view, SIGNAL ( doubleClicked ( const QModelIndex & ) ), this, SLOT ( Play() ) );
   connect ( view, SIGNAL ( pressed ( const QModelIndex & ) ), this, SLOT ( initLocation ( const QModelIndex & ) ) ); //pressed but not clicked! entered

}

void Player::setInfo ( ) {
   if ( !setInfoCheck ) {
      il -> addWidget ( trackTechInfo );
   }

   std::map < std::string, std::string > vorbisComm = ddata -> getVorbisComment();

   //BEGIN Удаление винжетов
   foreach ( QWidget * widget, qlhQueue ) {
      qlh -> removeWidget ( widget ) ;
      delete widget; widget = NULL;
   }
   qlhQueue . clear();
   foreach ( QWidget * widget, qlhQueueEdit ) {
      qlh -> removeWidget ( widget ) ;
      delete widget; widget = NULL;
   }
   qlhQueueEdit . clear();
   foreach ( QLayoutItem * item, qlQueue ) {
      ql -> removeItem ( item ) ;
      delete item; item = NULL;
   }
   qlQueue . clear();
   //END

   // Main metadata
   QChar fill = '0';
//    ql_fileName -> setText ( tr ( "Название файла: %1" ) . arg ( ( getFileNamefromFullPath ( this -> currFileName ) ) . c_str() ) );
   ql_sampleRate -> setText ( tr ( "Частота дискретизации: %1 Hz" ) . arg ( ddata -> getSampleRate() ) );
   ql_channels -> setText( tr ( "Количество каналов: %1" ) . arg ( ddata -> getChannels() ) );
   ql_bitPerSample -> setText ( tr ( "Бит на семпл: %1" ) . arg ( ddata -> getBitsPerSample() ) );
   ql_bitRate -> setText ( tr ( "Битрейт: %1 kbps" ) . arg ( ( ddata -> getBitRate() / 1000.0 ), 0, 'f', 1 ) );
   double time = ddata -> getTotalTime();
   ql_totalTime -> setText ( tr ( "Длительность: %1:%2" ) . arg ( static_cast < int > ( time ) / 60 ) . arg ( ( static_cast < int > ( time ) % 60 ), 2, 10, fill ) );
   ql_fileSize -> setText ( tr ( "Размер файла: %1 Mb" ) . arg ( ( ( ddata -> getFileSize() ) / 1048576.0 ), 0, 'f', 1 ) );

   /*
   Не обязательные параметры, то есть их может и не быть, поэтому проверяем
   */
   if ( ddata -> getBitsPerSample() != -1 ) {
      ql_bitPerSample -> show();
   } else {
      ql_bitPerSample -> hide();
   }

      // Vorbis Comments
   metaInfoLayout = new QGridLayout;
   metaInfoLayout -> addWidget ( metaInfoEditButton );
   
   bool cover = false;
   int i = 0;
   for ( std::map < std::string, std::string >::iterator it = vorbisComm . begin (  ); it != vorbisComm . end (  ); ++it ) {
      ql_VorbisComment_label [ it -> first ] = new QLabel ( tr ( "%1: " ) . arg ( QString ( ( it -> first ) . c_str() ) . toUpper() ) );
      qlhQueue . enqueue ( ql_VorbisComment_label [ it -> first ] );

      unsigned int found = it -> first . find ( "COVERART" );
      
      if ( found != std::string::npos ) {
//          std::cout << "first 'needle' found at: " << int(found) << std::endl;
         cover = true;
         break;
      } else if ( it -> first == "TRACKNUMBER" ) {
         ql_VorbisComment_spinBox [ it -> first ] = new QSpinBox;
         ql_VorbisComment_spinBox [ it -> first ] -> setValue ( QString ( ( it -> second ) . c_str() ) . toInt() );
         qlhQueueEdit . enqueue ( ql_VorbisComment_spinBox [ it -> first ] );
      } else {
         ql_VorbisComment_lineEdit [ it -> first ] = new QLineEdit ( vorbisComm [ it -> first ] . c_str() );
         qlhQueueEdit . enqueue ( ql_VorbisComment_lineEdit [ it -> first ] );
      }

      if ( this -> editMetaInfoCheck == false ) {
         qlhQueueEdit . last() -> setDisabled ( true );
      }

      metaInfoLayout -> addWidget ( ql_VorbisComment_label [ it -> first ], i + 1, 0 );
      metaInfoLayout -> addWidget ( qlhQueueEdit . last(), i + 1, 1 );

      ++i;
   }

   qlQueue . enqueue ( metaInfoLayout );
   qlm -> addLayout ( metaInfoLayout  );

   
//    std::fstream file_op ("png.png",std::ios::out);
//    file_op << this -> audio -> getPicture();
//    file_op.close();


   // Если есть обложка с META-данных или Vorbis tags то ставим ее
   std::multimap < ::FLAC__StreamMetadata_Picture_Type, std::string > picData = this -> ddata -> getPicture();
   if ( ( picData . empty() ) == false || cover == true ) {
      QPixmap * qpix = new QPixmap;
      if ( cover == true ) { // с Vorbis tags
         QByteArray ba ( ( ( Utils::Base64::base64_decode ( vorbisComm [ "COVERART" ] ) ) . c_str() ), ( Utils::Base64::base64_decode ( vorbisComm [ "COVERART" ] ) ) . size() );
         qpix -> loadFromData ( ba );
      } else { // c META-данных ( flac )
         std::cout << "COUNT: " << picData . size() << std::endl;
//          QByteArray ba ( this -> audio -> getPicture() . c_str(), this -> audio -> getPicture() . size() );
//          qpix -> loadFromData ( ba );
      }
      setCoverSize ( qpix );
      imageLabel -> setPixmap ( *qpix );
   } else { // если нету обложки, ставим свою
      imageLabel -> setPixmap ( QPixmap ( ":/img/player/nocover.png" ) );
      imageLabel -> setFixedSize ( 250, 250 );
   }
   setInfoCheck = true;
}

// Маштабирование обложки до размеров не больше this -> imageSize
void Player::setCoverSize ( const QPixmap * pix ) {
   // получаем значение ширины и высоты рисунка
   int width = pix -> width();
   int height = pix -> height();

   /*
   объявляем и инициализируем переменные значением ширины и высоты самого же рисунка
   в случае если изображение ( ширина и высота ) не превышает значения this -> imageSize
   */
   int widthK = width;
   int heightK = height;

   /*
   проверяем не превышает ли изображение значения this -> imageSize
   если да, то маштабируем его
   */
   if ( ( std::max ( width, height ) ) > ( this -> imageSize ) ) {
   double k = ( std::max ( width, height ) ) / ( this -> imageSize );
   widthK = width / k;
   heightK = height / k;
   }
   // устанавливаем фиксированый размер
   imageLabel -> setFixedSize ( widthK, heightK );
}

void Player::setSeek ( int val ) {
   if ( this -> globalSongStatus == SilentMedia::U ) {
      this -> Play();
      audio -> setSeek ( val, this -> tmpId );
   } else {
      audio -> setSeek ( val, this -> id );
   }
   this -> globalSongStatus = SilentMedia::Play; // меняем статус на Play
}

QWidget * Player::retW () {
   return playerTabWidget;
}

//BEGIN SLOTS
void Player::Play ( void ) {
   this -> playIndex = this -> index;
//    view -> blockSignals ( true ); // нуно это запустить как то раньше....
//    App app;
//    setTitle ( this -> currFileName );
//    setWindowTitle ( tr ( "SSNGMC" ) );
   if ( this -> globalSongStatus != SilentMedia::U ) {
      this -> audio -> stop ( this -> id );
   }
   this -> globalSongStatus = SilentMedia::Play; // меняем статус на Play
   this -> id = this -> currFileName;
   this -> audio -> play ( this -> currFileName, id );
   this -> setInfo();
   buttonPause -> setEnabled ( true );
}

void Player::Pause ( void ) {
   /*
   ** Пустословие, так как кнопка Pause и так блокируется в слоте this -> Stop() **
   Разрешаем использовать этот слот только когда песня стоит на паузе ( Pause ) или проигрывается ( Play ).
   В противном случае ( если она в состоянии Stop ) все игнорируется.
   */
   if ( this -> globalSongStatus == SilentMedia::Pause || this -> globalSongStatus == SilentMedia::Play ) {
      pauseCheck = !pauseCheck; // меняем состояние стража
      if ( pauseCheck ) { // если true значит песня вопроизводится ( Play )
         // ставим на паузу
         this -> audio -> stop ( this -> id );
         this -> globalSongStatus = SilentMedia::Pause;
      } else { // иначе она уже стоит на паузе, поэтому
         // ставим на продолжение
         this -> audio -> resume ( this -> id );
         this -> globalSongStatus = SilentMedia::Play;
      }
   }
}

void Player::Stop ( void ) {
   this -> globalSongStatus = SilentMedia::Stop; // меняем статус на Stop
   this -> audio -> stop ( this -> id );
   this -> audio -> flush ( this -> id );
   buttonPause -> setDisabled ( true );
}

void Player::Previous ( void ) {
   if ( ( index . row() ) > 0 ) { // пустословие. Уже блокируем в initLocation()
      QModelIndex index = this -> playIndex . sibling ( ( ( this -> playIndex . row() ) - 1 ), 1 );
      this -> view -> setCurrentIndex ( index );
      this -> initLocation ( index );
      this -> Play();
   }
}

void Player::Forward ( void ) {
   int row = this -> playIndex . row() + 1;
   if ( this -> maxRow >= row ) {
      QModelIndex index = this -> playIndex . sibling ( row , 1 );
      this -> view -> setCurrentIndex ( index );
      this -> initLocation ( index );
      this -> Play();
   }
}

void Player::initLocation ( const QModelIndex & index ) {
   // Выставляем состояние (вкл./выкл ) кнопки в зависимости от номера текущей строки. Если она 0 - то выкл.
   this -> buttonPrev -> setEnabled ( index . row() );

   this -> buttonForward -> setEnabled ( ( ( index . row() + 1 ) >= this -> maxRow ) ? false : true );

   this -> index = index;
   currFileName = index.sibling ( index.row(), 1 ) . data() . toString() . toStdString();
   this -> tmpId = currFileName; // обязательно локальная!
   if ( !this -> prevId . empty() ) {
      this -> audio -> closeF ( this -> prevId );
   }
   this -> audio -> openFile ( currFileName, this -> tmpId );
   setInfo();
   this -> prevId = this -> tmpId;
//    this -> audio -> flush();
   std::cout << currFileName << std::endl;
}

void Player::showCoverMenu ( const QPoint & p ) {
   QMenu menu ( this );
   menu . addAction ( loadCoverAct );
   menu . exec ( imageLabel -> mapToGlobal ( p ) );
}

// ф-ция для установки обложки
void Player::loadCover ( void ) {
   // получаем путь к файлу для загрузки
   std::string coverFileName = ( ( QFileDialog::getOpenFileName ( this, tr ( "Choose cover" ), QDir::currentPath() ) ) . toStdString() );
   
    // Проверяем выбрали ли мы какой-либо файл, иначе будет "Ошибка сегментирования"
   if ( !coverFileName.empty() ) {
   std::string coverData;
   std::ifstream coverDataFile ( coverFileName . c_str(), std::ios::binary ); // считываем наш файл

   // считываем посимвольно с coverDataFile в coverData
   while ( coverDataFile.good() ) {
      coverData += coverDataFile.get();
   }

   coverDataFile.close();

//    audio -> setCover ( coverData );
   }
}

void Player::editMETAComment ( void ) {

   foreach ( QWidget * widget, qlhQueueEdit ) {
      widget -> setDisabled ( editMetaInfoCheck );
   }

   if ( this -> editMetaInfoCheck == true ) {
      this -> editMetaInfoCheck = false;
   } else {
      this -> editMetaInfoCheck = true;
   }
}

void Player::rescanDb ( void ) {
   model -> clear();
   delete this -> model;
   this -> closeDbConn();
   scan -> flush();
   scan -> scan ( this -> settingsDbConn -> getVal ( "collectionPath" ) );
   this -> playListSetup();
}

void Player::updateSliderPos ( void ) {
   this -> posSlider -> setSliderPosition ( this -> audio -> getSeek ( this -> id ) );
}

void Player::disconnectSlider ( ) {
   timer -> stop ( );
}

// данный слот вызываеться при перемещении движка воспроизведения, а точнее при его оптускании ( Release )
void Player::connectSlider ( void ) {
   /*
   Этот код важен, для коректной работы плеера при перемещении движка воспроизведения в режиме паузы.
   Если песня стоит на паузе ее можна продолжить + установить на требуемое время лишь перемещением ползунка.
   Для этого нужно изменить состояние статуса в глобальном интерфейсе, а так же состояние стажа
   */
   if ( this -> globalSongStatus == SilentMedia::Pause ) {
//       audio -> changeStatus ( SilentMedia::Resume );
      pauseCheck = !pauseCheck; // меняем состояние стража
   }
   /*
   Меняем состояние глобального статуса, так как если движек был перемещен и отпущен, значит песня в состоянии вопроизведения ( Play ).
   Если же она была в состоянии паузы ( Pause ) то как раз поменяем этот состояние на Play
   */
//    this -> globalSongStatus = SilentMedia::Play;
   this -> setSeek ( this -> sliderPos );
   timer -> start ( 50 );
}

void Player::writeSliderPos ( int val ) {
//    timer -> stop ( );
   this -> sliderPos = val;
//    this -> setSeek ( this -> sliderPos );
//    timer -> start ( 50 );
}

void Player::openFileDialogFolder ( void ) {
   this -> collectionFolder = ( ( this -> fileDialog -> getExistingDirectory ( this, tr ( "Select collection folder" )
   , /*QDir::currentPath()*/ this -> settingsDbConn -> getVal ( "collectionPath" ) . c_str(), QFileDialog::ShowDirsOnly ) ) . toStdString() );
   this -> collectionLineEdit -> setText ( ( this -> collectionFolder ) . c_str() );
}

void Player::ApplySlot ( void ) {
   this -> settingsDbConn -> setVal ( "collectionPath", this -> collectionFolder );
}

void Player::OKSlot ( void ) {
   this -> ApplySlot();
   this -> preferentWidget -> close();
}

void Player::ReleaseMem ( void ) {
   
   
}

void Player::PreferentsWindow ( void ) {
   this -> preferentWidget = new QWidget;
   this -> preferentWidget -> setWindowTitle ( tr ( "Preferents" ) );
   this -> preferentWidget -> resize ( 800, 500 );
   
   QGridLayout * layout = new QGridLayout;
   QVBoxLayout * mainLayout = new QVBoxLayout;

   QHBoxLayout * bottomLayout = new QHBoxLayout;
   QPushButton * OK = new QPushButton ( tr ( "Ok" ) );
   QPushButton * Cancel = new QPushButton ( tr ( "Cancel" ) );
   QPushButton * Apply = new QPushButton ( tr ( "Apply" ) );
   QPushButton * Help = new QPushButton ( tr ( "Help" ) );
   connect ( OK, SIGNAL ( clicked() ), this, SLOT ( OKSlot() ) );
   connect ( Cancel, SIGNAL ( clicked() ), preferentWidget, SLOT ( close() ) );
   connect ( Apply, SIGNAL ( clicked() ), this, SLOT ( ApplySlot() ) );
   bottomLayout -> addWidget ( Help );
   bottomLayout -> addStretch();
   bottomLayout -> addWidget ( OK );
   bottomLayout -> addWidget ( Apply );
   bottomLayout -> addWidget ( Cancel );

   this -> fileDialog = new QFileDialog;
   QLabel * collectionLabel = new QLabel ( tr ( "Select the folder with the music collection" ) );
   QPushButton * collectionPushButton = new QPushButton ( tr ( "Open" ) );
   this -> collectionLineEdit = new QLineEdit ( this -> settingsDbConn -> getVal ( "collectionPath" ) . c_str() );
   connect ( collectionPushButton, SIGNAL ( clicked() ), this, SLOT ( openFileDialogFolder() ) );

   layout -> addWidget ( collectionLabel , 0, 0 );
   layout -> addWidget ( collectionLineEdit, 1, 0 );
   layout -> addWidget ( collectionPushButton, 1, 1 );

   mainLayout -> addLayout ( layout );
   mainLayout -> addLayout ( bottomLayout );
//    layout -> addWidget ( OK, 5, 0, 1, 1 );
//    layout -> addWidget ( Cancel, 5, 1, 1, 1 );
//    layout -> addWidget ( Apply, 5, 2, 1, 1 );
//    layout->setMargin(1);
//    layout->setSpacing(1);
//    layout->setColumnStretch(1, 1);
//    layout->setColumnStretch(10, 1);
//    layout->setRowStretch(5, 1);
   
   this -> preferentWidget -> setLayout ( mainLayout );
   this -> preferentWidget -> show();
}

//END SLOTS
