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

#ifndef _ROTESONNE_UI_WIDGETS_MIXER_OSS_MIXER_HPP_
#define _ROTESONNE_UI_WIDGETS_MIXER_OSS_MIXER_HPP_

#include <include.hpp>
#include <libsml/all.hpp>

#include <QtGui>
#include "Peak.hpp"

using namespace std;

//class SilentMedia::Media::Audio::SoundSystem::OSS::Mixer::Mixer;
class Peak;

namespace RoteSonne {
  namespace UI {
    namespace Widgets {
      namespace Mixer {
        namespace OSS {
          class Mixer: public QMainWindow {
            Q_OBJECT
            public:
              Mixer();
              ~Mixer();
              QWidget * retW(void);
            private:
              void init();
              void prepareStyle();
              void createActions();
              void createMenus();
              void initScan(bool rescan, int countElem);

              // stylize
              QString CSS_RED_SLIDER;
              QString CSS_BLACK_SLIDER;
              QString CSS_ON_LABEL;
              QString CSS_OFF_LABEL;
              QString CSS_MUTE_CHECKBOX;
              QString CSS_UNMUTE_CHECKBOX;

              libconfig::Config cfg;
              SilentMedia::Media::Audio::SoundSystem::OSS::Mixer::Mixer
                  * ossmix;
              Peak *peak;

              QVBoxLayout * mainCtrlLayout;

              QMap<int, QToolButton *> ImgButton;
              map<int, string> listOfCtrl;
              string ListOfDevice;

              int numRecDev;
              bool recModeAvail;
              bool recModeStatus;
              bool stereo;
              bool on;
              int L;
              int R;
              int M;
              int minCtrlValue;
              int maxCtrlValue;
              string ctrlLabel;
              int ctrlFlag;

              int coutOfCtrlEl;

              string currentEnumName;
              int currentEnumNum;

              int currentRecId;

              fstream * ptmpConfigFile;
              fstream * pConfigFile;

              string currentConfigFile;
              string configFile;
              int updateCounter;
              QMap<int, qint64> iconCacheSum;
              QMap<int, bool> ctrlStatus;
              QTimer *timer;

              QActionGroup *alignmentGroup;

              QLabel *infoLabel;

              QSignalMapper *sliderSignalMapperL;
              QSignalMapper *sliderSignalMapperR;
              QSignalMapper *sliderSignalMapperM;
              QSignalMapper *muteLRActSignalMapper;
              QSignalMapper *muteLActSignalMapper;
              QSignalMapper *muteRActSignalMapper;
              QSignalMapper *muteMActSignalMapper;
              QSignalMapper *setRecordSrcSignalMapper;
              QSignalMapper *reverseActSignalMapper;
              QSignalMapper *splitChanelActSignalMapper;
              QSignalMapper *setCheckedControlSignalMapper;
              QSignalMapper *setEnumControlSignalMapper;

              map<int, int> IdtoDevNum;
              map<int, int> vIdtoCtrlRecNum;
              map<int, int> mctrlMode;
              map<int, int> ctrlMinVal;
              map<int, int> ctrlMaxVal;
              map<int, string> IdtoPicName;

              QMap<int, QIcon *> icon;

              QMap<int, QSlider *> sliderL;
              QMap<int, QSlider *> sliderR;
              QMap<int, QSlider *> sliderM;

              QMap<int, QLineEdit *> labelL;
              QMap<int, QLineEdit *> labelR;
              QMap<int, QLineEdit *> labelM;

              QMap<int, QCheckBox *> muteLCheckBox;
              QMap<int, QCheckBox *> muteRCheckBox;
              QMap<int, QCheckBox *> muteMCheckBox;

              QMap<int, QAction *> muteLRAct;

              QMap<int, QAction *> reverseAct;
              QMap<int, QAction *> splitChanelAct;
              QMap<int, QAction *> setRecordSrcAct;
              QMap<int, QComboBox *> enumComboBox;
              QMap<int, QCheckBox *> onOffCheckbox;
              QMap<int, QLCDNumber *> lcdL;
              QMap<int, QLCDNumber *> lcdR;
              QMap<int, QLCDNumber *> lcdM;
              QMap<int, QLabel *> mixDevLabel;

              map<int, string> enumListVariant;
              int ctrlTypeName;

              QLabel *l3;

              QTextEdit *console;
              QTabWidget *mainTabWidget;
              QTabWidget *mixerTabWidget;
              string currentSong;

              QMap<int, QVBoxLayout *> vImageLayout;

              QMap<int, QVBoxLayout *> vLayoutSliderLCDL;
              QMap<int, QVBoxLayout *> vLayoutSliderLCDR;
              QMap<int, QVBoxLayout *> vLayoutSliderLCDM;

              QHBoxLayout *infoLayer;
              QHBoxLayout *consoleLayer;

              QGroupBox *secondGroupBox;
              QGroupBox *onoffGroupBox;

              QMap<int, QHBoxLayout *> layoutCtrl;
              QGridLayout *layoutCtrl2;

              QVBoxLayout *Layout;
              QVBoxLayout *Layout2;
              QVBoxLayout *Layout3;
              QVBoxLayout *Layout4;

              unsigned int parentNum;
              unsigned int prevParentNum;
              unsigned int nextParentNum;

              unsigned int currentParent;
              QMap<int, QHBoxLayout *> currentParentNum;

              vector<int> listOfPeak;
              map<int, int> maxValList;
              map<int, int> peakLevelL;
              map<int, int> peakLevelR;

              QWidget *widget4;
              QWidget *widget5;
              QFrame *frame1;

              QImage *image;
              QPushButton *pb;
              QLabel *tmpL;
              QLabel *l;
              QPixmap *qp;

              private slots:
              void setIconStatus ( int i, bool status );
              void initSliderLabel(int i, bool ctrlMode, int ctrlTypeName);
              void rescanCtrlDev();

              void update();

              void setVol(const int id, bool mode, int &setVolL, int &setVolR,
                  int &setVolM, int &devNum);
              void setvolL(int id);
              void setvolR(int id);
              void setvolM(int id);
              void aboutCardInfo();

              void muteL(int id);
              void muteR(int id);
              void muteLR(int id);
              void muteM(int id);
              void muteControl(int id, bool L, bool R, bool M);
              void reverse(int id);
              void setCheckedControl(int id);
              void setEnumControl(int id);
              void saveConfig();
              void setRecCtrl(int id);
              void splitChanel(int id);

signals          :
          void setvolI ( int );

        };
      }
    }
  }
}
}

#endif
