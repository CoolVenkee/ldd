TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    secondtest.c \
    globalmemtest.c \
    globalfifotest.c \
    i2ctest.c \
    eepromtest.c \
    pwmtest.c

HEADERS += \
    lddtest.h

