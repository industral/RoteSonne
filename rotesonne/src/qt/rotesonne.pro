QMAKE_CXXFLAGS += -ggdb
QMAKE_CFLAGS += -ggdb

CONFIG      += uitools
QT          += sql
RESOURCES   = rotesonne.qrc

HEADERS     = src/UI/Widgets/MainWindow/MainWindow_UI.hpp \
              src/UI/Widgets/MainWindow/Widgets/TrackInfo_UI.hpp \
              src/UI/Widgets/MainWindow/Widgets/PlayList_UI.hpp \
              src/UI/Widgets/Collection/Collection_UI.hpp \
              src/Collection.hpp

SOURCES     = src/UI/LoadUI.cpp \
              src/Player.cpp \
              src/UI/Widgets/MainWindow/MainWindow_UI.cpp \
              src/UI/Widgets/MainWindow/Widgets/TrackInfo_UI.cpp \
              src/UI/Widgets/MainWindow/Widgets/PlayList_UI.cpp \
              src/UI/Widgets/Collection/Collection_UI.cpp \
              src/common.cpp \
              src/Collection.cpp
              
LIBS += -lsml -lvorbisfile -lFLAC++ -lwavpack -lao -lboost_thread-mt -lboost_filesystem-mt
