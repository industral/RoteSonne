CONFIG      += uitools qt warn_on DEBUG_MODE #RELEASE_MODE
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
              src/UI/Widgets/Preferences/Preferences_UI.hpp \
              #src/UI/Widgets/Mixer/OSS/Mixer.hpp \
              #src/UI/Widgets/Mixer/OSS/Peak.hpp \
              src/Collection.hpp \
              src/StartUp.hpp \
              src/UI/Window.hpp \
              src/Configuration.hpp \
              src/include.hpp

SOURCES     = src/UI/LoadUI.cpp \
              src/UI/Window.cpp \
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
              src/UI/Widgets/Preferences/Preferences_UI.cpp \
              #src/UI/Widgets/Mixer/OSS/Mixer.cpp \
              #src/UI/Widgets/Mixer/OSS/Peak.cpp \
              src/common.cpp \
              src/StartUp.cpp \
              src/Collection.cpp \
              src/Configuration.cpp

Libraries = sml vorbisfile FLAC++ wavpack ao asound xspf boost_thread-mt \
  boost_filesystem-mt config++ uriparser log4cxx

StaticLibraries = sml vorbisfile FLAC++ FLAC vorbis ogg wavpack ao asound xspf \
  boost_thread-mt boost_system-mt boost_filesystem-mt config++ uriparser log4cxx

CONFIG(DEBUG_MODE) {
  QMAKE_CXXFLAGS += -ggdb -Wall -W
  QMAKE_CFLAGS += -ggdb -Wall -W

  for(lib, Libraries) {
    LIBS += -l$$lib
  }
} else {
  QMAKE_CXXFLAGS += -O3
  QMAKE_CFLAGS += -O3
  
  for(lib, StaticLibraries) {
    LIBS += "/usr/lib/lib$$lib".a
  }
}

INCLUDEPATH = src
OBJECTS_DIR = tmp/objects
MOC_DIR = tmp/moc
RCC_DIR =  tmp/rcc
