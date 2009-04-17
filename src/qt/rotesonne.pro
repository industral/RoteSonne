CONFIG      += uitools

RESOURCES   = rotesonne.qrc

HEADERS     = src/ui/MainWindow_UI.hpp \
              src/ui/Collection_UI.hpp

SOURCES     = src/LoadUI.cpp \
              src/ui/MainWindow_UI.cpp \
              src/ui/Collection_UI.cpp \
              src/common.cpp \
              src/Collection.cpp
              
LIBS += -lsml -lvorbisfile -lao -lsqlite3