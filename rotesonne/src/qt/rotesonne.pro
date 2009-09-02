QMAKE_CXXFLAGS += -ggdb -Wall -W
QMAKE_CFLAGS += -ggdb -Wall -W

CONFIG      += uitools
QT          += sql
RESOURCES   = rotesonne.qrc

HEADERS     = src/UI/Widgets/MainWindow/MainWindow_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/TrackInfo_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Cover_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Library/Library_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/TrackList_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/ArtistList_UI.hpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/AlbumList_UI.hpp \
              src/UI/Widgets/Collection/Collection_UI.hpp \
              src/Collection.hpp \
              src/StartUp.hpp

SOURCES     = src/UI/LoadUI.cpp \
              src/Player.cpp \
              src/UI/Widgets/MainWindow/MainWindow_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/TrackInfo_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Cover_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/Library/Library_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/TrackList_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/ArtistList_UI.cpp \
              src/UI/Widgets/MainWindow/Main/Widgets/PlayList/AlbumList_UI.cpp \
              src/UI/Widgets/Collection/Collection_UI.cpp \
              src/common.cpp \
              src/StartUp.cpp \
              src/Collection.cpp
              
LIBS += -lsml -lvorbisfile -lFLAC++ -lwavpack -lao -lboost_thread-mt -lboost_filesystem-mt -lasound -lxspf
