CONFIG -= qt

QMAKE_LFLAGS += -mwindows -static-libgcc -flto
QMAKE_CFLAGS += -fno-asynchronous-unwind-tables
LIBS += -luser32

SOURCES += main.c
