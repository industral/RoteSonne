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

#include "Mixer.hpp"

string
    _redSlider_ =
        "QSlider::groove { \
      background: url(:/img/slider.png); \
      background-position: bottom center; \
      background-attachment: fixed; \
      background-repeat: repeat-y;}\
      QSlider::handle { \
      image: url(:/img/Slider-red.png);}";

string _ledGreenEnable_ = "background-color: rgb(0, 192, 0);"
  "border: 1px inset #201F1F;";

string _ledGreenDisable_ = "background-color: rgb(0, 48, 0);"
  "border: 1px inset #201F1F;";

string _ledYellowEnable_ = "background-color: rgb(255, 255, 0);"
  "border: 1px inset #201F1F;";

string _ledYellowDisable_ = "background-color: rgb(64, 64, 0);"
  "border: 1px inset #201F1F;";

string _ledBlueEnable_ = "background-color: rgb(51, 51, 255);"
  "border: 1px inset #201F1F;";

string _ledBlueDisable_ = "background-color: rgb(13, 13, 64);"
  "border: 1px inset #201F1F;";

string _ledBrownEnable_ = "background-color: rgb(128, 0, 0);"
  "border: 1px inset #201F1F;";

string _ledBrownDisable_ = "background-color: rgb(32, 0, 0);"
  "border: 1px inset #201F1F;";

string _ledRedEnable_ = "background-color: rgb(255, 0, 0);"
  "border: 1px inset #201F1F;";

string _ledRedDisable_ = "background-color: rgb(64, 0, 0);"
  "border: 1px inset #201F1F;";

//  "background-color: #1D7BE7;"
// #201F1F #16DC24 #1ABAFF

#define _onLabel_ \
"background-color: #1ABAFF;" \
"border: 1px inset #201F1F;" \
"font: bold;"
//////////////////////
#define _offLabel_ \
"background-color: #B5B5B5;" \
"border: 1px inset #201F1F;" \
"font: bold;"
//////////////////////
#define _blackSlider_ \
"QSlider::groove {" \
"background: url(:/img/slider.png);"\
"background-position: bottom center;"\
"background-attachment: fixed;"\
"background-repeat: repeat-y;}"\
\
"QSlider::handle {" \
"image: url(:/img/Slider.png);}"
////////////////////
#define _muteCheckBox_ \
"QCheckBox {" \
"spacing: 5px;" \
"}" \
\
"QCheckBox::indicator {" \
"width: 17px;" \
"height: 17px;" \
"}" \
\
"QCheckBox::indicator:checked {" \
"image: url(:/img/off/record.png);" \
"}" \
\
"QCheckBox::indicator:unchecked {" \
"image: url(:/img/on/record.png);" \
"}"

#define _onoffCheckBox_ \
"QCheckBox {" \
"spacing: 5px;" \
"}" \
\
"QCheckBox::indicator {" \
"width: 17px;" \
"height: 17px;" \
"}" \
\
"QCheckBox::indicator:checked {" \
"image: url(:/img/on/record.png);" \
"}" \
\
"QCheckBox::indicator:unchecked {" \
"image: url(:/img/off/record.png);" \
"}"
//////////////////////
#define _onoff2CheckBox_ \
"QCheckBox {" \
"spacing: 5px;" \
"}" \
\
"QCheckBox::indicator {" \
"width: 13px;" \
"height: 13px;" \
"}" \
\
"QCheckBox::indicator:checked {" \
"image: url(:/img/check.png);" \
"}"

SSMix::SSMix() :
  ossmix(SilentMedia::SoundSystem::OSS::Mixer::Instance()), /*sscc ( new SScc ),*/
  peak(new Peak), ctrlNum(-1), numRecDev(-1), ctrlParent(-1), recModeAvail(0),
      recModeStatus(0), ctrlMode(0), ctlStatus(-1), L(-1), R(-1), M(-1),
      minCtrlValue(-1), maxCtrlValue(-1), skipDev(0), ctrlFlag(-1),
      currentEnumNum(-1), updateCounter(-1), currentParent(-1) {
}

SSMix::~SSMix() {
  for (unsigned short int id = 0; id != coutOfCtrlEl; ++id) {
    //       if ( img [ id ] ) { delete img [ id ]; }
    if (sliderL[id]) {
      delete sliderL[id];
    }
    if (sliderR[id]) {
      delete sliderR[id];
    }
    if (sliderM[id]) {
      delete sliderM[id];
    }
    if (muteLRAct[id]) {
      delete muteLRAct[id];
    }
    if (reverseAct[id]) {
      delete reverseAct[id];
    }
    if (splitChanelAct[id]) {
      delete splitChanelAct[id];
    }
    if (setRecordSrcAct[id]) {
      delete setRecordSrcAct[id];
    }
    if (enumComboBox[id]) {
      delete enumComboBox[id];
    }
    if (onOffCheckbox[id]) {
      delete onOffCheckbox[id];
    }
    if (ImgButton[id]) {
      delete ImgButton[id];
    }
    if (labelL[id]) {
      delete labelL[id];
    }
    if (labelR[id]) {
      delete labelR[id];
    }
    if (labelM[id]) {
      delete labelM[id];
    }

    delete mixDevLabel[id];

    //   delete vImageLayout [ id ];
    delete vLayoutSliderLCDL[id];
    delete vLayoutSliderLCDR[id];
    delete vLayoutSliderLCDM[id];
  }
  //    delete sscc;
  delete ossmix;
  delete pConfigFile;
  delete ptmpConfigFile;
}

void SSMix::init() {
  currentConfigFile = "current.xml";
  configFile = "config.xml";
  //
  //  Glib::ustring filepath;
  //  filepath = configFile;
  //
  //   // Parse the entire document in one go:
  //  try {
  //   MySaxParser parser;
  //   parser.set_substitute_entities(true); //
  //   parser.parse_file(filepath);
  //  }
  //  catch(const xmlpp::exception& ex) {
  //   cout << "libxml++ exception: " << ex.what() << endl;
  //  }
  // 	pb = new QPushButton ( "test buton" );
  // 	l = new QLabel;
  // 	tmpL = new QLabel/* ( "asdas" )*/;

  timer = new QTimer(this);
  pConfigFile = new fstream;
  ptmpConfigFile = new fstream;

  ptmpConfigFile -> open(currentConfigFile.c_str(), fstream::trunc
      | fstream::in | fstream::out);
  *ptmpConfigFile << "<mixer>" << endl;

  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForTr(codec);

  //    sscc -> init ( OS ( GNULinux ) );
  ossmix -> init("/dev/mixer");

  updateCounter = ossmix -> getUpdateCounter();

  ossmix -> returnListOfAvaibleControlDev(listOfAvaibleCtrlDev);

  // 	layoutCtrl2 = new QHBoxLayout;
  layoutCtrl2 = new QGridLayout;
  layoutCtrl[2] = new QHBoxLayout;
  layoutCtrl[3] = new QHBoxLayout;
  layoutCtrl[4] = new QHBoxLayout;
  layoutCtrl[5] = new QHBoxLayout;
  //   layoutCtrl [ 6 ] = new QHBoxLayout;
  layoutCtrl[7] = new QHBoxLayout;

  //  QTabWidget *tabWidget = new QTabWidget;
  //  tabWidget->addTab(this, tr("General"));
  //  MainLayout -> addWidget ( tabWidget );

  coutOfCtrlEl = listOfAvaibleCtrlDev . size();

  sliderSignalMapperL = new QSignalMapper;
  sliderSignalMapperR = new QSignalMapper;
  sliderSignalMapperM = new QSignalMapper;

  muteLRActSignalMapper = new QSignalMapper;
  muteLActSignalMapper = new QSignalMapper;
  muteRActSignalMapper = new QSignalMapper;
  muteMActSignalMapper = new QSignalMapper;
  setRecordSrcSignalMapper = new QSignalMapper;
  reverseActSignalMapper = new QSignalMapper;
  splitChanelActSignalMapper = new QSignalMapper;

  // ctlStatus/OFF ENUM
  setCheckedControlSignalMapper = new QSignalMapper;
  setEnumControlSignalMapper = new QSignalMapper;

  peak = new Peak();
  peak -> resize(240, 120);

  initScan(0, 0);

  connect ( sliderSignalMapperL, SIGNAL ( mapped ( int ) ), this, SLOT ( setvolL ( int ) ) );
  connect ( sliderSignalMapperR, SIGNAL ( mapped ( int ) ), this, SLOT ( setvolR ( int ) ) );
  connect ( sliderSignalMapperM, SIGNAL ( mapped ( int ) ), this, SLOT ( setvolM ( int ) ) );
  connect ( muteLRActSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( muteLR ( int ) ) );
  connect ( muteLActSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( muteL ( int ) ) );
  connect ( muteRActSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( muteR ( int ) ) );
  connect ( muteMActSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( muteM ( int ) ) );
  connect ( reverseActSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( reverse ( int ) ) );
  connect ( splitChanelActSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( splitChanel ( int ) ) );
  connect ( setRecordSrcSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( setRecCtrl ( int ) ) );
  connect ( setCheckedControlSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( setCheckedControl ( int ) ) );
  connect ( setEnumControlSignalMapper, SIGNAL ( mapped ( int ) ), this, SLOT ( setEnumControl ( int ) ) );

  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer -> start(50);

  //  tabWidget -> addTab ( new GeneralTab (fileInfo), tr("General"));

  QGroupBox *mainGroupBox = new QGroupBox(tr("Main Control"));
  mainGroupBox -> setLayout(layoutCtrl2);

  secondGroupBox = new QGroupBox(tr("mixext Control"));
  secondGroupBox -> setLayout(layoutCtrl[2]);

  onoffGroupBox = new QGroupBox(tr("mixext Control"));
  onoffGroupBox -> setLayout(layoutCtrl[3]);

  QGroupBox *thirdGroupBox = new QGroupBox(tr("vmix0-out"));
  thirdGroupBox -> setLayout(layoutCtrl[4]);

  QGroupBox *GroupBox4 = new QGroupBox(tr("Sound Card Info"));
  GroupBox4 -> setLayout(layoutCtrl[5]);

  //   QGroupBox *GroupBox5 = new QGroupBox ( tr ( "Console Input" ) );
  //   GroupBox5 -> setLayout ( layoutCtrl [ 6 ] );

  // 	QGroupBox *GroupBox6 = new QGroupBox ( tr ( "Peak Output" ) );
  // 	GroupBox6 -> setLayout ( layoutCtrl [ 7 ] );

  //                                                             Layout = new QVBoxLayout;
  Layout2 = new QVBoxLayout;
  Layout3 = new QVBoxLayout;
  Layout4 = new QVBoxLayout;

  QWidget *widget2 = new QWidget;
  QWidget *widget3 = new QWidget;
  widget4 = new QWidget;
  widget5 = new QWidget;

  Layout2 -> addWidget(mainGroupBox);
  // 	Layout2 -> addWidget ( GroupBox4 );
  // 	Layout2 -> addWidget ( peak );
  // 	peak -> show();
  Layout3 -> addWidget(secondGroupBox);
  Layout3 -> addWidget(onoffGroupBox);
  Layout3 -> addWidget(thirdGroupBox);

  widget2 -> setLayout(Layout2);
  widget3 -> setLayout(Layout3);

  mixerTabWidget = new QTabWidget;
  mixerTabWidget -> addTab(widget2, QString("Main Controls"));
  mixerTabWidget -> addTab(widget3, QString("Output Controls"));
  mixerTabWidget -> addTab(peak, QString("Output Peaks"));
}

void SSMix::initScan(int action, int countElem) {

  if (action) {
    for (int id = 0; id != countElem; ++id) {
      //          delete img [ id ]; img [ id ] = 0;
      delete ImgButton[id];
      ImgButton[id] = 0;
      delete muteLRAct[id];
      muteLRAct[id] = 0;
      delete reverseAct[id];
      reverseAct[id] = 0;
      delete splitChanelAct[id];
      splitChanelAct[id] = 0;
      delete setRecordSrcAct[id];
      setRecordSrcAct[id] = 0;
      delete labelL[id];
      labelL[id] = 0;
      delete labelR[id];
      labelR[id] = 0;
      delete labelM[id];
      labelM[id] = 0;
      delete sliderL[id];
      sliderL[id] = 0;
      delete sliderR[id];
      sliderR[id] = 0;
      delete sliderM[id];
      sliderM[id] = 0;
      delete mixDevLabel[id];
      mixDevLabel[id] = 0;
      delete enumComboBox[id];
      enumComboBox[id] = 0;
      delete onOffCheckbox[id];
      onOffCheckbox[id] = 0;
    }
  }

  int id = 0;

  for (map <int, string>::iterator it = listOfAvaibleCtrlDev.begin(); it
      != listOfAvaibleCtrlDev.end(); ++it) {

    ossmix -> getDevInfo(it -> second, ctrlLabel, ctrlNum, ctrlParent,
        numRecDev, recModeAvail, recModeStatus, ctrlMode, ctlStatus, L, R, M,
        minCtrlValue, maxCtrlValue, skipDev, enumListVariant, currentEnumName,
        currentEnumNum, ctrlTypeName, ctrlFlag);

    if (!skipDev) {
      *ptmpConfigFile << "\t" << "<control id='" << ctrlNum << "'>"
          << endl << "\t\t" << "<name>" << ctrlLabel << "</name>"
          << endl << "\t\t" << "<type>" << ctrlTypeName << "</type>"
          << endl << "\t\t" << "<chanels>" << "merge" << "</chanels>"
          << endl << "\t\t" << "<record>" << recModeStatus << "</record>"
          << endl << "\t\t" << "<mute>" << endl << "\t\t\t" << "<L>"
          << "0" << "</L>" << endl << "\t\t\t" << "<R>" << "0" << "</R>"
          << endl << "\t\t" << "</mute>" << endl << "\t\t"
          << "<value>" << endl << "\t\t\t" << "<L>" << L << "</L>"
          << endl << "\t\t\t" << "<R>" << R << "</R>" << endl
          << "\t\t" << "</value>" << endl << "\t" << "</control>"
          << endl;

      IdtoDevNum[id] = ctrlNum;
      cout << id << " : " << ctrlNum << endl;
      vIdtoCtrlRecNum[id] = numRecDev;
      mctrlMode[id] = ctrlMode;
      ctrlMinVal[id] = minCtrlValue;
      ctrlMaxVal[id] = maxCtrlValue;

      //   cout << "MIXT: " << ctrlTypeName << endl;
      //   cout << "label: " << ctrlLabel << endl;
      //   cout << "ctrlMode: " << ctrlMode << endl << endl;

      if (recModeStatus) {
        currentRecId = id;
      }
      // MIXT_HEXVALUE MIXT_VALUE
      //BEGIN MIXT_STEREOSLIDER && MIXT_STEREODB && MIXT_MONOSLIDER && MIXT_MONODB && MIXT_MONOVU && MIXT_SLIDER && MIXT_STEREOSLIDER16 && MIXT_MONOSLIDER16
      if ((ctrlTypeName == MIXT_STEREOSLIDER)
          || (ctrlTypeName == MIXT_STEREODB) || (ctrlTypeName
          == MIXT_MONOSLIDER) || (ctrlTypeName == MIXT_MONODB) || (ctrlTypeName
          == MIXT_MONOVU) || (ctrlTypeName == MIXT_SLIDER) || (ctrlTypeName
          == MIXT_STEREOSLIDER16) || (ctrlTypeName == MIXT_MONOSLIDER16)) {

        string loadImg;

        if (ctrlLabel == "mic") {
          loadImg = ":/img/audio-input-microphone.png";
        } else if (ctrlLabel == "cd") {
          loadImg = ":/img/media-optical-audio.png";
        } else if (ctrlLabel == "phone") {
          loadImg = ":/img/audio-headset.png";
        } else if (ctrlFlag & MIXF_MAINVOL) {
          loadImg = ":/img/speaker.png";
        } else if ((ctrlTypeName == MIXT_STEREOSLIDER16) || (ctrlTypeName
            == MIXT_MONOSLIDER16)) {
          loadImg = ":/img/account_offline_overlay.png";
        } else if (ctrlFlag & MIXF_PCMVOL) {
          loadImg = ":/img/media-podcast.png";
        } else if (ctrlLabel == "video") {
          loadImg = ":/img/camera-web.png";
        } else if (ctrlFlag & MIXF_RECVOL) {
          loadImg = ":/img/media-record.png";
        } else {
          loadImg = ":/img/audio-input-line.png";
        }

        IdtoPicName[id] = loadImg;
        // ImgButton [ id ] должен быть инициализирован раньше, так, как используется в ф-ции setIconStatus ( int id, bool status );
        ImgButton[id] = new QToolButton;
        icon[id] = new QIcon;

        if (ctrlMode) {
          if (L == minCtrlValue && R == minCtrlValue) {
            // Ставим в true а не в false чтобы поменять ( поставить ) иконку.
            ctrlStatus[id] = true;
            setIconStatus(id, false);
            // Так как установка статуса иконки выставляется впервые, записываем в ctrlStatus [ id ] статус.
            // В дальнейшем, мы будем проверять его, не изменился ли он в setIconStatus ( int id, bool status ),
            // чтобы лишний раз не менять значение иконки активные / неактивный стан.
            //                   ctrlStatus [ id ] = false;
          } else if (L != minCtrlValue || R != minCtrlValue) {
            ctrlStatus[id] = false;
            setIconStatus(id, true);
          } /*else {*/
          //                   loadImg = "account_offline_overlay.png";
          //                   IdtoPicName [ id ] = loadImg;
          //                   cout << sscc -> P ( Color ( red ) ) << "[ QSSMix ] [ ERROR: ] [ Value of `" << ctrlLabel << "` control = " << L << " : " << R << " ] " << sscc -> P ( Color ( white ) ) << endl;
          // #ifdef QSSMIX_DEBUG
          //                   cout << endl << sscc -> P ( Color ( cyan ) ) << "[ QSSMix ] [ DEBUG MODE START: ]" << endl;
          //                   cout << "\t" << "Control name: " << ctrlLabel << endl;
          //                   cout << "\t" << "Control type: " << ctrlTypeName << endl;
          //                   cout << "\t" << "Control number: " << ctrlNum << endl;
          //                   cout << "\t" << "Record mode availability: " << recModeAvail << endl;
          //                   cout << "\t" << "Record mode status: " << recModeStatus << endl;
          //                   cout << "\t" << "MONO / STEREO: " << ctrlMode << endl;
          //                   cout << "[ QSSMix ] [ DEBUG MODE END. ]" << sscc -> P ( Color ( white ) ) << endl << endl;
          // #endif
          //                }
        } else if (!ctrlMode) {
          if (M == minCtrlValue) {
            ctrlStatus[id] = true;
            setIconStatus(id, false);
          } else if (M != minCtrlValue) {
            ctrlStatus[id] = false;
            setIconStatus(id, true);
          }/* else {
           loadImg = "account_offline_overlay.png";
           IdtoPicName [ id ] = loadImg;
           cout << sscc -> P ( Color ( red ) ) << "[ QSSMix ] [ ERROR: ] [ Value of `" << ctrlLabel << "` control = " << M << " ]" << sscc -> P ( Color ( white ) ) << endl;
           #ifdef QSSMIX_DEBUG
           cout << endl << sscc -> P ( Color ( cyan ) ) << "[ QSSMix ] [ DEBUG MODE START: ]" << endl;
           cout << "\t" << "Control name: " << ctrlLabel << endl;
           cout << "\t" << "Control type: " << ctrlTypeName << endl;
           cout << "\t" << "Control number: " << ctrlNum << endl;
           cout << "\t" << "Record mode availability: " << recModeAvail << endl;
           cout << "\t" << "Record mode status: " << recModeStatus << endl;
           cout << "\t" << "MONO / STEREO: " << ctrlMode << endl;
           cout << "[ QSSMix ] [ DEBUG MODE END. ]" << sscc -> P ( Color ( white ) ) << endl << endl;
           #endif
           }*/
        }

        ImgButton[id] -> setIcon(*icon[id]);
        ImgButton[id] -> setIconSize(QSize(32, 32));
        ImgButton[id] -> setPalette(QColor(127, 127, 127, 150));
        ImgButton[id] -> setPopupMode(QToolButton::InstantPopup);
        ImgButton[id] -> setFocusPolicy(Qt::NoFocus);

        if (recModeStatus) {
          // Устанавливаем ImgButton [ i ] красного цвета, если контролер является источком записи
          ImgButton[id] -> setPalette(QColor(238, 7, 7, 150));
        }

        if (ctrlMode) {
          muteLRAct[id] = new QAction(QIcon(":/img/mute.png"), tr("&Mute"),
              this);
          reverseAct[id] = new QAction(QIcon(":/img/xfce4_xicon.png"), tr(
              "&Reverse chanels"), this);
          splitChanelAct[id] = new QAction(QIcon(":/img/xfce4_xicon.png"), tr(
              "&Split chanels"), this);

          muteLRAct[id] -> setCheckable(true);
          reverseAct[id] -> setCheckable(true);

          ImgButton[id] -> addAction(muteLRAct[id]);
          ImgButton[id] -> addAction(reverseAct[id]);

          splitChanelAct[id] -> setCheckable(true);
          ImgButton[id] -> addAction(splitChanelAct[id]);

          muteLCheckBox[id] = new QCheckBox(this);
          if (L == minCtrlValue) {
            muteLCheckBox[id] -> setChecked(true);
            muteLCheckBox[id] -> setDisabled(true);
          }
          muteLCheckBox[id] -> setFocusPolicy(Qt::NoFocus);
          muteLCheckBox[id] -> setStyleSheet(_muteCheckBox_);

          muteRCheckBox[id] = new QCheckBox(this);
          if (R == minCtrlValue) {
            muteRCheckBox[id] -> setChecked(true);
            muteRCheckBox[id] -> setDisabled(true);
          }
          muteRCheckBox[id] -> setFocusPolicy(Qt::NoFocus);
          muteRCheckBox[id] -> setStyleSheet(_muteCheckBox_);

        } else if (!ctrlMode) {
          muteMCheckBox[id] = new QCheckBox(this);
          if (M == minCtrlValue) {
            muteMCheckBox[id] -> setChecked(true);
            muteMCheckBox[id] -> setDisabled(true);
          }
          muteMCheckBox[id] -> setFocusPolicy(Qt::NoFocus);
          muteMCheckBox[id] -> setStyleSheet(_muteCheckBox_);
        }

        if (recModeAvail) {
          setRecordSrcAct[id] = new QAction(QIcon(":/img/rec.png"), tr(
              "&Set record source"), this);
          // 						setRecordSrcAct [ id ] -> setCheckable ( true );
          ImgButton[id] -> addAction(setRecordSrcAct[id]);
        }

        if (ctrlMode) {
          connect ( muteLCheckBox [ id ] , SIGNAL ( stateChanged ( int ) ), muteLActSignalMapper, SLOT ( map ( ) ) );
          muteLActSignalMapper -> setMapping(muteLCheckBox[id], id);

          connect ( muteRCheckBox [ id ] , SIGNAL ( stateChanged ( int ) ), muteRActSignalMapper, SLOT ( map ( ) ) );
          muteRActSignalMapper -> setMapping(muteRCheckBox[id], id);

          connect(reverseAct[id], SIGNAL(triggered()), reverseActSignalMapper,
              SLOT(map()));
          reverseActSignalMapper -> setMapping(reverseAct[id], id);

          connect(splitChanelAct[id], SIGNAL(triggered()),
              splitChanelActSignalMapper, SLOT(map()));
          splitChanelActSignalMapper -> setMapping(splitChanelAct[id], id);

        } else if (!ctrlMode) {
          connect ( muteMCheckBox [ id ] , SIGNAL ( stateChanged ( int ) ), muteMActSignalMapper, SLOT ( map ( ) ) );
          muteMActSignalMapper -> setMapping(muteMCheckBox[id], id);
        }

        if (recModeAvail) {
          connect(setRecordSrcAct[id], SIGNAL(triggered()),
              setRecordSrcSignalMapper, SLOT(map()));
          setRecordSrcSignalMapper -> setMapping(setRecordSrcAct[id], id);
        }

        initSliderLabel(id, ctrlMode, ctrlTypeName);

        mixDevLabel[id] = new QLabel;
        mixDevLabel[id] -> setText((ctrlLabel) . c_str());

        if (ctrlMode) {
          connect ( sliderL [ id ], SIGNAL ( valueChanged ( int ) ), sliderR [ id ], SLOT ( setValue ( int ) ) );

          connect ( sliderL [ id ], SIGNAL ( valueChanged ( int ) ), sliderSignalMapperL, SLOT ( map ( ) ) );
          sliderSignalMapperL -> setMapping(sliderL[id], id);

          connect ( sliderR [ id ], SIGNAL ( valueChanged ( int ) ), sliderL [ id ], SLOT ( setValue ( int ) ) );

          connect ( sliderR [ id ], SIGNAL ( valueChanged ( int ) ), sliderSignalMapperR, SLOT ( map ( ) ) );
          sliderSignalMapperR -> setMapping(sliderR[id], id);
        }

        if (!ctrlMode) {
          connect ( sliderM [ id ], SIGNAL ( valueChanged ( int ) ), sliderSignalMapperM, SLOT ( map ( ) ) );
          sliderSignalMapperM -> setMapping(sliderM[id], id);
        }

        mainCtrlLayout = new QVBoxLayout;
        vImageLayout[id] = new QVBoxLayout;
        vImageLayout[id] -> addWidget(ImgButton[id]);
        vImageLayout[id] -> setAlignment(Qt::AlignHCenter);

        vLayoutSliderLCDL[id] = new QVBoxLayout;
        vLayoutSliderLCDR[id] = new QVBoxLayout;
        vLayoutSliderLCDM[id] = new QVBoxLayout;

        QHBoxLayout *hLayoutSliderLCD = new QHBoxLayout;

        if (ctrlMode) {
          vLayoutSliderLCDL[id] -> addWidget(labelL[id]);
          vLayoutSliderLCDL[id] -> addWidget(sliderL[id]);
          vLayoutSliderLCDL[id] -> addWidget(muteLCheckBox[id]);
          vLayoutSliderLCDR[id] -> addWidget(labelR[id]);
          vLayoutSliderLCDR[id] -> addWidget(sliderR[id]);
          vLayoutSliderLCDR[id] -> addWidget(muteRCheckBox[id]);

          hLayoutSliderLCD -> addLayout(vLayoutSliderLCDL[id]);
          hLayoutSliderLCD -> addLayout(vLayoutSliderLCDR[id]);
        } else if (!ctrlMode) {
          vLayoutSliderLCDM[id] -> addWidget(labelM[id]);
          vLayoutSliderLCDM[id] -> addWidget(sliderM[id]);
          vLayoutSliderLCDM[id] -> addWidget(muteMCheckBox[id]);
          hLayoutSliderLCD -> addLayout(vLayoutSliderLCDM[id]);
        }

        mainCtrlLayout -> addLayout(vImageLayout[id]);
        mainCtrlLayout -> addLayout(hLayoutSliderLCD);

        QVBoxLayout *tmpLayout = new QVBoxLayout;
        tmpLayout -> addLayout(mainCtrlLayout);

        QGroupBox *tmpGroupBox = new QGroupBox(ctrlLabel . c_str());
        tmpGroupBox -> setLayout(tmpLayout);

        QVBoxLayout *tmpLayout2 = new QVBoxLayout;
        tmpLayout2 -> addWidget(tmpGroupBox);

        static int il = 0;

        if (id == 8) {
          il = 0;
        }

        if ((ctrlTypeName == MIXT_STEREOSLIDER16) || (ctrlTypeName
            == MIXT_MONOSLIDER16)) {
          // 						cout << "16 " << id << endl;
          layoutCtrl[4] -> addLayout(tmpLayout2);
        } else {
          if (id > 7) {
            layoutCtrl2 -> addLayout(tmpLayout2, 1, il);
          } else {
            layoutCtrl2 -> addLayout(tmpLayout2, 0, il);
          }
          ++il;
        }
      }

      //END MIXT_STEREOSLIDER && MIXT_STEREODB && MIXT_MONOSLIDER && MIXT_MONODB && MIXT_MONOVU && MIXT_SLIDER && MIXT_STEREOSLIDER16 && MIXT_MONOSLIDER16

      //BEGIN MIXT_ENUM && MIXT_ONOFF
      else if ((ctrlTypeName == MIXT_ENUM) || (ctrlTypeName == MIXT_ONOFF)) {

        if (!skipDev) {
          QHBoxLayout *secondLayout = new QHBoxLayout;
          QHBoxLayout *thirdLayout = new QHBoxLayout;

          if (ctrlTypeName == MIXT_ENUM) {
            enumComboBox[id] = new QComboBox;

            for (unsigned short int j = 0; j != enumListVariant.size(); ++j) {
              enumComboBox[id] -> addItem(QString(enumListVariant[j].c_str()));
            }
            enumComboBox[id] -> setCurrentIndex(currentEnumNum);

            connect ( enumComboBox [ id ] , SIGNAL ( currentIndexChanged ( int ) ), setEnumControlSignalMapper, SLOT ( map ( ) ) );
            setEnumControlSignalMapper -> setMapping(enumComboBox[id], id);
            secondLayout -> addWidget(enumComboBox[id]);
          }

          if (ctrlTypeName == MIXT_ONOFF) {
            onOffCheckbox[id] = new QCheckBox((ctrlLabel) . c_str(), this);
            onOffCheckbox[id] -> setStyleSheet(_onoffCheckBox_);
            onOffCheckbox[id] -> setFocusPolicy(Qt::NoFocus);

            onOffCheckbox[id] -> setChecked(currentEnumNum);

            connect ( onOffCheckbox [ id ] , SIGNAL ( stateChanged ( int ) ), setCheckedControlSignalMapper, SLOT ( map ( ) ) );
            setCheckedControlSignalMapper -> setMapping(onOffCheckbox[id], id);

            thirdLayout -> addWidget(onOffCheckbox[id]);
          }
          layoutCtrl[2] -> addLayout(secondLayout);
          layoutCtrl[3] -> addLayout(thirdLayout);

        }
      }
      //END MIXT_ENUM && MIXT_ONOFF

      //BEGIN MIXT_STEREOVU && MIXT_STEREOPEAK
      else if ((ctrlTypeName == MIXT_STEREOVU) || (ctrlTypeName
          == MIXT_STEREOPEAK)) {
        if (!skipDev) {
          listOfPeak . push_back(ctrlNum);
          maxValList[ctrlNum] = maxCtrlValue;
        }
      }
      //END MIXT_STEREOVU && MIXT_STEREOPEAK
      id++;
    }
  }

  // l -> setPixmap ( *( peak -> returnPeakImage ( ) ) );

  *ptmpConfigFile << "</mixer>";
  *ptmpConfigFile << endl;
  ptmpConfigFile -> close();

  ////////////////////////
  QLabel *l1 = new QLabel;
  QLabel *l2 = new QLabel;
  l3 = new QLabel;

  //    l1 -> setText ( ( ossmix -> getChipsetName ( ) ) . c_str ( ) );
  //    l2 -> setText ( QString ( "%1 v%2 %3 License. %1 API v%4" ) . arg ( ( ossmix -> getProductName ( ) ) . c_str ( ) ) . arg ( ( ossmix -> getProductVersion ( ) ) . c_str ( ) )
  //          . arg ( ( ossmix -> getProductLicense ( ) ) . c_str ( ) ) . arg ( ossmix -> getProductAPI ( ), 0, 16 ) );
  //    l1 -> setAlignment ( Qt::AlignHCenter | Qt::AlignVCenter );
  //    l2 -> setAlignment ( Qt::AlignHCenter | Qt::AlignVCenter );

  QPushButton *sndCardButton = new QPushButton(this);
  sndCardButton -> setIcon(QIcon(":/img/audio-card.png"));
  sndCardButton -> setIconSize(QSize(64, 64));

  infoLayer = new QHBoxLayout;
  infoLayer -> addWidget(sndCardButton, 1);
  infoLayer -> addWidget(l1, 1);
  infoLayer -> addWidget(l2, 3);
  infoLayer -> addWidget(l3, 2);

  layoutCtrl[5] -> addLayout(infoLayer);

  connect(sndCardButton, SIGNAL(clicked()), this, SLOT(aboutCardInfo()));
  ///////////////////////////

  //   console = new QTextEdit;
  // //   console -> setBackgroundRole ( QPalette::Background );
  // //   QTextEdit::focusInEvent(e);
  // //   keyPressEvent(e);
  //   console -> installEventFilter(this);
  //   console -> setText ( QString ( "qssmix $ " ) );
  // //   console -> key (  );
  //
  // //   connect ( console, SIGNAL ( textChanged (  ) ), this, SLOT ( eventFilter (  ) ) );
  //
  //   consoleLayer = new QHBoxLayout;
  //   consoleLayer -> addWidget ( console );
  //
  //   layoutCtrl [ 6 ] -> addLayout ( consoleLayer );
}

//  bool SSMix::eventFilter(QObject *target, QEvent *event) {
//    if (event->type() == QEvent::KeyPress) {
//     QKeyEvent *keyEvent = (QKeyEvent *)event;
//     if ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) {
//      console -> setText ( QString ( "sdg " ) );
//      console -> installEventFilter(this);
//      console -> setText ( QString ( "qssmix $ " ) );
//         }
//    }
//   return QWidget::eventFilter(target, event);
//  }


void SSMix::setIconStatus(int id, bool status) {
  if (ctrlStatus[id] != status) {
    ctrlStatus[id] = status;
    status ? icon[id] -> addPixmap(
        (QIcon((IdtoPicName[id]) . c_str())) . pixmap(QSize(32, 32),
            QIcon::Normal, QIcon::On)) : icon[id] -> addPixmap((QIcon(
        (IdtoPicName[id]) . c_str())) . pixmap(QSize(32, 32), QIcon::Disabled,
        QIcon::On));
    ImgButton[id] -> setIcon(*icon[id]);
  }
}

// void SSMix::contextMenuEvent ( QContextMenuEvent *event ) {
//    QMenu menu ( this );
//    menu.addAction ( cutAct );
//    menu.addAction ( copyAct );
//    menu.addAction ( pasteAct );
//    menu.exec ( event->globalPos() );
// }

void SSMix::initSliderLabel(int id, bool ctrlMode, int ctrlTypeName) {
  if (ctrlMode) {
    labelL[id] = new QLineEdit(QString::number(L));
    labelL[id] -> setDisabled(true);
    labelL[id] -> setFixedWidth(30);
    labelL[id] -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if (L != minCtrlValue) {
      labelL[id] -> setStyleSheet(_onLabel_);
    } else {
      labelL[id] -> setStyleSheet(_offLabel_);
    }
    labelR[id] = new QLineEdit(QString::number(R));
    labelR[id] -> setDisabled(true);
    labelR[id] -> setFixedWidth(30);
    labelR[id] -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if (R != minCtrlValue) {
      labelR[id] -> setStyleSheet(_onLabel_);
    } else {
      labelR[id] -> setStyleSheet(_offLabel_);
    }
  } else if (!ctrlMode) {
    labelM[id] = new QLineEdit(QString::number(M));
    labelM[id] -> setDisabled(true);
    labelM[id] -> setFixedWidth(30);
    labelM[id] -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if (M != minCtrlValue) {
      labelM[id] -> setStyleSheet(_onLabel_);
    } else {
      labelM[id] -> setStyleSheet(_offLabel_);
    }
  }

  string sliderStyle;
  if ((ctrlTypeName == MIXT_STEREOSLIDER16) || (ctrlTypeName
      == MIXT_MONOSLIDER16)) {
    sliderStyle = _redSlider_;
  } else {
    sliderStyle = _blackSlider_;
  }

  if (ctrlMode) {
    sliderL[id] = new QSlider;
    sliderL[id] -> setStyleSheet(sliderStyle . c_str());

    sliderL[id] -> setRange(minCtrlValue, maxCtrlValue);
    sliderL[id] -> setSliderPosition(L);

    sliderR[id] = new QSlider;
    sliderR[id] -> setStyleSheet(sliderStyle . c_str());

    sliderR[id] -> setRange(minCtrlValue, maxCtrlValue);
    sliderR[id] -> setSliderPosition(R);
  } else if (!ctrlMode) {
    sliderM[id] = new QSlider;
    sliderM[id] -> setStyleSheet(sliderStyle . c_str());
    sliderM[id] -> setRange(minCtrlValue, maxCtrlValue);
    sliderM[id] -> setSliderPosition(M);
  }
}

void SSMix::setVol(const int id, bool mode, int &setVolL, int &setVolR,
    int &setVolM, int &devNum) {
  devNum = IdtoDevNum[id];

  if (!mode) {
    setVolM = sliderM[id] -> value();

    if (setVolM != ctrlMinVal[id]) {
      setIconStatus(id, true);
      if ((labelM[id] -> styleSheet()) != _onLabel_) {
        labelM[id] -> setStyleSheet(_onLabel_);
      }
      if (muteMCheckBox[id] -> isChecked()) {
        muteMCheckBox[id] -> setChecked(false);
      }
      if (!(muteMCheckBox[id] -> isEnabled())) {
        muteMCheckBox[id] -> setDisabled(false);
      }
    } else if (setVolM == ctrlMinVal[id]) {
      setIconStatus(id, false);
      labelM[id] -> setStyleSheet(_offLabel_);
      muteMCheckBox[id] -> setChecked(true);
      muteMCheckBox[id] -> setDisabled(true);
    }
  } else {
    setVolL = sliderL[id] -> value();
    setVolR = sliderR[id] -> value();

    if (setVolL != ctrlMinVal[id]) {
      if ((labelL[id] -> styleSheet()) != _onLabel_) {
        labelL[id] -> setStyleSheet(_onLabel_);
      }
      if (muteLCheckBox[id] -> isChecked()) {
        muteLCheckBox[id] -> setChecked(false);
      }
      if (!(muteLCheckBox[id] -> isEnabled())) {
        muteLCheckBox[id] -> setDisabled(false);
      }
    } else if (setVolL == ctrlMinVal[id]) {
      if ((labelL[id] -> styleSheet()) != _offLabel_) {
        labelL[id] -> setStyleSheet(_offLabel_);
      }
      muteLCheckBox[id] -> setChecked(true);
      muteLCheckBox[id] -> setDisabled(true);
    }

    if (setVolR != ctrlMinVal[id]) {
      if ((labelR[id] -> styleSheet()) != _onLabel_) {
        labelR[id] -> setStyleSheet(_onLabel_);
      }
      if (muteRCheckBox[id] -> isChecked()) {
        muteRCheckBox[id] -> setChecked(false);
      }
      if (!(muteRCheckBox[id] -> isEnabled())) {
        muteRCheckBox[id] -> setDisabled(false);
      }
    } else if (setVolR == ctrlMinVal[id]) {
      if ((labelR[id] -> styleSheet()) != _offLabel_) {
        labelR[id] -> setStyleSheet(_offLabel_);
      }
      muteRCheckBox[id] -> setChecked(true);
      muteRCheckBox[id] -> setDisabled(true);
    }

    if ((setVolL != ctrlMinVal[id]) && (setVolR != ctrlMinVal[id])) {
      setIconStatus(id, true);
    } else if ((setVolL == ctrlMinVal[id]) && (setVolR == ctrlMinVal[id])) {
      setIconStatus(id, false);
    }
  }
}

void SSMix::stuff() {
  //    for ( int i = 80; i != 0; --i ) {
  //       ossmix -> setDevVol ( 2, i, i, -1 );
  //       cout << i << endl;
  //    }
  //    for ( int i = 0; i != 80; ++i ) {
  //       ossmix -> setDevVol ( 2, i, i, -1 );
  //       cout << i << endl;
  //    }
}

void SSMix::update() {
  //	ossdsp -> getSong ( currentSong );
  l3 -> setText(QString("Current song: %1") . arg(currentSong . c_str()));
  l3 -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  int L = -1;
  int R = -1;
  int numLedL = -1;
  int numLedR = -1;
  for (unsigned short int i = 0; i != listOfPeak . size(); ++i) {
    ossmix -> getPeak(listOfPeak[i], L, R);

    if (!R || !L) {
      if (!L) {
        numLedL = 0;
      }
      if (!R) {
        numLedR = 0;
      }
    } else {
      numLedL = (9 / (maxValList[listOfPeak[i]] / static_cast <double> (L)));
      numLedR = (9 / (maxValList[listOfPeak[i]] / static_cast <double> (R)));
    }
    peakLevelL[i] = numLedL;
    peakLevelR[i] = numLedR;
  }
  peak -> drawPeak(peakLevelL, peakLevelR);
  // 	vLayoutSliderLCDL [ 35 ] -> update();
  // 	*qp = ( *( peak -> returnPeakImage ( ) ) );

  // 	QWidget *w = new QWidget;
  // 	w -> render ( peak -> returnPeakImage ( ) );
  // 	l -> setUpdatesEnabled(true);

  // 	l -> setWindowModified ( true );


  // peak -> update();
  // 	l -> repaint();

  if (ossmix -> getUpdateCounter() > updateCounter) {
    updateCounter = ossmix -> getUpdateCounter();

    int id = 0;
    for (map <int, string>::iterator it =
        listOfAvaibleCtrlDev.begin(); it != listOfAvaibleCtrlDev.end(); ++it) {
      ossmix -> getDevInfo(it->second, ctrlLabel, ctrlNum, ctrlParent,
          numRecDev, recModeAvail, recModeStatus, ctrlMode, ctlStatus, L, R, M,
          minCtrlValue, maxCtrlValue, skipDev, enumListVariant,
          currentEnumName, currentEnumNum, ctrlTypeName, ctrlFlag);

      if (!skipDev) {
        if ((ctrlTypeName == MIXT_STEREOSLIDER) || (ctrlTypeName
            == MIXT_STEREODB) || (ctrlTypeName == MIXT_MONOSLIDER)
            || (ctrlTypeName == MIXT_MONODB) || (ctrlTypeName == MIXT_MONOVU)
            || (ctrlTypeName == MIXT_SLIDER)) {

          if (ctrlMode) {
            sliderL[id] -> setSliderPosition(L);
            sliderR[id] -> setSliderPosition(R);
          } else {
            sliderM[id] -> setSliderPosition(M);
          }
        } else if (ctrlTypeName == MIXT_ENUM) {
          enumComboBox[id] -> setCurrentIndex(currentEnumNum);
        } else if (ctrlTypeName == MIXT_ONOFF) {
          onOffCheckbox[id] -> setChecked(currentEnumNum);
        } else if ((ctrlTypeName == MIXT_STEREOSLIDER16) || (ctrlTypeName
            == MIXT_MONOSLIDER16)) {
          if (ctrlMode) {
            sliderL[id] -> setSliderPosition(L);
            sliderR[id] -> setSliderPosition(R);
          } else {
            sliderM[id] -> setSliderPosition(M);
          }
        }
        ++id;
      }
    }
  }
}

void SSMix::setvolL(int id) {
  int devNum = -1;
  int setVolL = -1;
  int setVolR = -1;
  int setVolM = -1;

  // Устанавливаем значение громкости
  setVol(id, true, setVolL, setVolR, setVolM, devNum);
  labelL[id] -> setText(QString::number(setVolL));
  ossmix -> setDevVol(devNum, setVolL, -1, -1);
}

void SSMix::setvolR(int id) {
  int devNum = -1;
  int setVolL = -1;
  int setVolR = -1;
  int setVolM = -1;

  setVol(id, true, setVolL, setVolR, setVolM, devNum);
  labelR[id] -> setText(QString::number(setVolR));
  ossmix -> setDevVol(devNum, -1, setVolR, -1);
}

void SSMix::setvolM(int id) {
  int devNum = -1;
  int setVolL = -1;
  int setVolR = -1;
  int setVolM = -1;

  setVol(id, false, setVolL, setVolR, setVolM, devNum);
  labelM[id] -> setText(QString::number(setVolM));
  ossmix -> setDevVol(devNum, -1, -1, setVolM);
}

void SSMix::muteL(int id) {
  SSMix::muteControl(id, 1, 0, 0);
  // Нужно для того, чтобы при выключении ползунок не сползал до нуля
  updateCounter = ossmix -> getUpdateCounter();
}

void SSMix::muteR(int id) {
  SSMix::muteControl(id, 0, 1, 0);
  updateCounter = ossmix -> getUpdateCounter();
}

void SSMix::muteLR(int id) {
  SSMix::muteControl(id, 1, 1, 0);
  updateCounter = ossmix -> getUpdateCounter();
}

void SSMix::muteM(int id) {
  SSMix::muteControl(id, 0, 0, 1);
  updateCounter = ossmix -> getUpdateCounter();
}

void SSMix::reverse(int id) {
  int devNum = IdtoDevNum[id];
  int setVol = sliderR[id] -> value();
  ossmix -> setDevVol(devNum, -1, setVol, -1);
}

void SSMix::muteControl(int id, bool L, bool R, bool M) {
  int devNum = IdtoDevNum[id];

  QMap <int, QCheckBox *> target;

  if (L && R) {
    //    target = muteLRAct;
    if (target[id] -> isChecked()) {

      labelL[id] -> setStyleSheet(_offLabel_);
      labelR[id] -> setStyleSheet(_offLabel_);
    } else {

      labelL[id] -> setStyleSheet(_onLabel_);
      labelR[id] -> setStyleSheet(_onLabel_);
    }
  } else if (L && !R) {
    ;
    target = muteLCheckBox;
    if (target[id] -> isChecked()) {
      labelL[id] -> setStyleSheet(_offLabel_);
    } else {
      labelL[id] -> setStyleSheet(_onLabel_);
    }
  } else if (!L && R) {
    target = muteRCheckBox;
    if (target[id] -> isChecked()) {
      labelR[id] -> setStyleSheet(_offLabel_);
    } else {
      labelR[id] -> setStyleSheet(_onLabel_);
    }
  } else if (M) {
    target = muteMCheckBox;
    if (target[id] -> isChecked()) {
      labelM[id] -> setStyleSheet(_offLabel_);
    } else {
      labelM[id] -> setStyleSheet(_onLabel_);
    }
  }

  if (target[id] -> isChecked()) {
    ossmix -> onOffDev(devNum, 0, L, R, M);
  } else {
    ossmix -> onOffDev(devNum, 1, L, R, M);
  }
}

void SSMix::setCheckedControl(int id) {
  int devNum = IdtoDevNum[id];
  ossmix -> changeDevState(devNum, onOffCheckbox[id] -> checkState());
}

void SSMix::setEnumControl(int id) {
  int devNum = IdtoDevNum[id];
  ossmix -> changeDevState(devNum, enumComboBox[id] -> currentIndex());
}

void SSMix::setRecCtrl(int id) {
  int devNum = vIdtoCtrlRecNum[id];
  ossmix -> changeDevState(devNum, 1);
  ImgButton[id] -> setPalette(QColor(238, 7, 7, 150));
  ImgButton[currentRecId] -> setPalette(QColor(127, 127, 127, 150));
  currentRecId = id;
}

void SSMix::splitChanel(int id) {
  if (splitChanelAct[id] ->isChecked()) {
disconnect  ( sliderL [ id ], SIGNAL ( valueChanged ( int ) ), sliderR [ id ], SLOT ( setValue ( int ) ) );
  disconnect ( sliderR [ id ], SIGNAL ( valueChanged ( int ) ), sliderL [ id], SLOT ( setValue ( int ) ) );
} else {
  connect ( sliderL [ id ], SIGNAL ( valueChanged ( int ) ), sliderR [ id ], SLOT ( setValue ( int ) ) );
  connect ( sliderR [ id ], SIGNAL ( valueChanged ( int ) ), sliderL [ id], SLOT ( setValue ( int ) ) );
}
}

void SSMix::aboutCardInfo() {
  //    QMessageBox msgBox;
  //    msgBox . setIconPixmap ( QPixmap ( ":/img/audio-card.png" ) );
  //    msgBox . addButton ( QMessageBox::Ok );
  //    msgBox . setWindowTitle ( tr ( "Информация о звуковой карте %1" ) . arg ( ossmix -> getSndCardName ( ) . c_str ( ) ) );
  //    msgBox . setText ( tr (
  //                      "<i> Звуковая карта: </i> %1"
  //          "<p><i> Звуковой чипсет: </i> %2"
  //          "<p><i> Устройство в системе: </i> %3"
  //          "<p><i> Минимальная частота дискретизации: </i> %4 Hz"
  //          "<p><i> Максимальная частота дискретизации: </i> %5 Hz"
  //          "<p><i> Минимальное количество каналов: </i> %6"
  //          "<p><i> Максимальное количество каналов: </i> %7"
  //          "<p><i> Идентификатор карты: </i> %8"
  //                         ) . arg ( ossmix -> getSndCardName ( ) . c_str ( ) ) . arg ( ossmix -> getChipsetName ( ) . c_str ( ) )
  //          . arg ( ossmix -> getDevNode ( ) . c_str ( ) ) . arg ( ossmix -> getMinSampleRate ( ) )
  //          . arg ( ossmix -> getMaxSampleRate ( ) ) . arg ( ossmix -> getMinCh ( ) ) . arg ( ossmix -> getMaxCh ( ) ) . arg ( ossmix -> getIdentSndCard ( ) . c_str ( ) ) );
  //    msgBox . exec ( );
}

void SSMix::saveConfig() {
  ifstream infile(currentConfigFile.c_str(), ios_base::binary);
  ofstream outfile(configFile.c_str(), ios_base::binary);
  outfile << infile.rdbuf();
}

void SSMix::rescanCtrlDev() {
  initScan(1, coutOfCtrlEl);
}

QWidget * SSMix::retW(void) {
  return this -> mixerTabWidget;
}
