CONFIG      += uitools
QT          += sql
RESOURCES   = rotesonne.qrc

HEADERS     = src/UI/Widgets/MainWindow_UI.hpp \
              src/UI/Widgets/Collection_UI.hpp

SOURCES     = src/UI/LoadUI.cpp \
              src/Player.cpp \
              src/UI/Widgets/MainWindow_UI.cpp \
              src/UI/Widgets/Collection_UI.cpp \
              src/common.cpp \
              src/Collection.cpp
              
LIBS += -lsml -lvorbisfile -lao -lsqlite3 -lboost_thread-mt
