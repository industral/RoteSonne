QMAKE_CXXFLAGS += -ggdb -Wall -W
QMAKE_CFLAGS += -ggdb -Wall -W

CONFIG      += uitools
QT          += sql
RESOURCES   = rotesonne.qrc

HEADERS     = src/UI/Widgets/MainWindow/MainWindow_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/TrackInfo_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Cover_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Library/Library_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/TrackList/TrackList_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/TrackList/CustomQSqlTableModel.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/ArtistList_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/AlbumList_UI.hpp \
              src/UI/Widgets/Collection/Collection_UI.hpp \
              #src/UI/Widgets/Mixer/OSS/Mixer.hpp \
              #src/UI/Widgets/Mixer/OSS/Peak.hpp \
              src/Collection.hpp \
              src/StartUp.hpp \
              src/Configuration.hpp \
              src/include.hpp

SOURCES     = src/UI/LoadUI.cpp \
              src/Player.cpp \
              src/UI/Widgets/MainWindow/MainWindow_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/TrackInfo_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Cover_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Library/Library_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/TrackList/TrackList_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/TrackList/CustomQSqlTableModel.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/ArtistList_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/AlbumList_UI.cpp \
              src/UI/Widgets/Collection/Collection_UI.cpp \
              #src/UI/Widgets/Mixer/OSS/Mixer.cpp \
              #src/UI/Widgets/Mixer/OSS/Peak.cpp \
              src/common.cpp \
              src/StartUp.cpp \
              src/Collection.cpp \
              src/Configuration.cpp
              
LIBS += -lsml -lvorbisfile -lFLAC++ -lwavpack -lao -lboost_thread-mt -lboost_filesystem-mt -lasound -lxspf -lconfig++
INCLUDEPATH = src
