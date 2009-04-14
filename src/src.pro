QMAKE_CXXFLAGS += -ggdb
QMAKE_CFLAGS += -ggdb

# $$system(Magick++-config --cppflags | cut -b3-)

TEMPLATE = app
TARGET = ssngmc
DEPENDPATH += .
INCLUDEPATH += . /usr/include/libsml /usr/include/ImageMagick
LIBS += -lsml -lvorbisfile -lFLAC++ -lwavpack -lsqlite3 -lfreetype -lz -lMagick++ -lMagickWand -lMagickCore -lasound -llua
QT += sql

# Input
HEADERS += app.h peak.h player.h scanc.h sqlite.h ssnnmc.h
SOURCES += app.cpp main.cpp peak.cpp player.cpp scanc.cpp sqlite.cpp ssnnmc.cpp
RESOURCES += ssnnmc.qrc
