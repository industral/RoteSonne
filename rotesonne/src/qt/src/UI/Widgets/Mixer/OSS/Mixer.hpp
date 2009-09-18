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

#ifndef SSNNMC_H
#define SSNNMC_H

#include <QtGui>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "app.h"
#include "peak.h"

// #include "myparser.h"

// using namespace sscc;

namespace SilentMedia {
   namespace SoundSystem {
      namespace OSS {
            class Mixer;
      }
   }
}

class Peak;
 
 class SSMix : public QMainWindow {
	Q_OBJECT

	public:
		SSMix ( );
		~SSMix ( );
                void init (  );
                QWidget * retW ( void );
		
	protected:
// 		void contextMenuEvent(QContextMenuEvent *event);
// 		void paintEvent ( QPaintEvent * );
		QMap < int, QToolButton * > ImgButton;
	private:
		void createActions();
		void createMenus();

       SilentMedia::SoundSystem::OSS::Mixer * ossmix;
//		libssoss::DSP *ossdsp;
// 		SScc *sscc;
		Peak *peak;
// 		Player *player;

		std::map < int, std::string > listOfAvaibleCtrlDev;
		std::string ListOfDevice;
		std::string DEV_NAME;

		int ctrlNum;
		int numRecDev;
		unsigned int ctrlParent;
		bool recModeAvail;
		bool recModeStatus;
		short int ctrlMode;
		short int ctlStatus;
		int L;
		int R;
		int M;
		int minCtrlValue;
		int maxCtrlValue;
		bool skipDev;
		std::string ctrlLabel;
		int ctrlFlag;

		int coutOfCtrlEl;
		
		std::string currentEnumName;
		int currentEnumNum;
		
		int currentRecId;
      
// 		FILE * ptmpConfigFile;
// 		FILE * pConfigFile;
		
		std::fstream * ptmpConfigFile;
		std::fstream * pConfigFile;
		
		std::string currentConfigFile;
		std::string configFile;
		int updateCounter;
      QMap < int, qint64 > iconCacheSum;
      QMap < int, bool > ctrlStatus;
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
		
		std::map < int, int > IdtoDevNum;
		std::map < int, int > vIdtoCtrlRecNum;
		std::map < int, int > mctrlMode;
		std::map < int, int > ctrlMinVal;
		std::map < int, int > ctrlMaxVal;
		std::map < int, std::string > IdtoPicName;
		
//       QMap < int, QPixmap * > img;
      QMap < int, QIcon * > icon;
		
      QMap < int, QSlider * > sliderL;
      QMap < int, QSlider * > sliderR;
      QMap < int, QSlider * > sliderM;
      
//       QMap < int, QLCDNumber * > labelL;
//       QMap < int, QLCDNumber * > labelR;
//       QMap < int, QLCDNumber * > labelM;
      
      QMap < int, QLineEdit * > labelL;
      QMap < int, QLineEdit * > labelR;
      QMap < int, QLineEdit * > labelM;
		
      QMap < int, QCheckBox * > muteLCheckBox;
      QMap < int, QCheckBox * > muteRCheckBox;
      QMap < int, QCheckBox * > muteMCheckBox;

      QMap < int, QAction * > muteLRAct;

      QMap < int, QAction * > reverseAct;
      QMap < int, QAction * > splitChanelAct;
      QMap < int, QAction * > setRecordSrcAct;
      QMap < int, QComboBox * > enumComboBox;
      QMap < int, QCheckBox * > onOffCheckbox;
//       QMap < int, QToolButton * > ImgButton;
      QMap < int, QLCDNumber * > lcdL;
      QMap < int, QLCDNumber * > lcdR;
      QMap < int, QLCDNumber * > lcdM;
      QMap < int, QLabel * > mixDevLabel;
	
		std::map < int, std::string > enumListVariant;
		int ctrlTypeName;
		
		QLabel *l3;
		
		QTextEdit *console;
		QTabWidget *mainTabWidget;
		QTabWidget *mixerTabWidget;
		std::string currentSong;
		
		QVBoxLayout *mainCtrlLayout;
      QMap < int, QVBoxLayout * > vImageLayout;
		
      QMap < int, QVBoxLayout * > vLayoutSliderLCDL;
      QMap < int, QVBoxLayout * > vLayoutSliderLCDR;
      QMap < int, QVBoxLayout * > vLayoutSliderLCDM;
		
		QHBoxLayout *infoLayer;
		QHBoxLayout *consoleLayer;
		
		QGroupBox *secondGroupBox;
		QGroupBox *onoffGroupBox;

		QMap < int, QHBoxLayout * > layoutCtrl;
		QGridLayout *layoutCtrl2;

		QVBoxLayout *Layout;
		QVBoxLayout *Layout2;
		QVBoxLayout *Layout3;
		QVBoxLayout *Layout4;
		
		unsigned int parentNum;
		unsigned int prevParentNum;
		unsigned int nextParentNum;
		
		unsigned int currentParent;
      QMap < int, QHBoxLayout * > currentParentNum;
		
		std::vector < int > listOfPeak;
		std::map < int, int > maxValList;
std::map < int, int > peakLevelL;
std::map < int, int > peakLevelR;

QWidget *widget4;
QWidget *widget5;
QFrame *frame1;

//       QPainter *painter;
//       QPixmap *myPixmap;
      QImage *image;
QPushButton *pb;
QLabel *tmpL;
QLabel *l;
QPixmap *qp;

// 		std::map < int, int > ctrlChanelMode;
	private slots:
      void setIconStatus ( int i, bool status );
// 		bool eventFilter(QObject *target, QEvent *event);
//       void DrawMe();
		void initSliderLabel ( int i, bool ctrlMode, int ctrlTypeName );
		void initScan ( int scan, int countElem );
		void rescanCtrlDev (  );
		void stuff();
		
// 		void setPeak ( int i, int j, bool status );
		void update (  );
		
		
		void setVol ( const int id, bool mode, int &setVolL, int &setVolR, int &setVolM, int &devNum );
		void setvolL ( int id );
		void setvolR ( int id );
		void setvolM ( int id );
		void aboutCardInfo (  );
		
		void muteL ( int id );
		void muteR ( int id );
		void muteLR ( int id );
		void muteM ( int id );
		void muteControl ( int id, bool L, bool R, bool M );
		void reverse ( int id );
		void setCheckedControl ( int id );
		void setEnumControl ( int id );
		void saveConfig (  );
		void setRecCtrl ( int id );
		void splitChanel ( int id );
		
	signals:
		void setvolI ( int );
		
};

#endif
