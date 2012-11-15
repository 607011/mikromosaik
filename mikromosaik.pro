# Copyright (c) 2012 Oliver Lau <ola@ct.de>

QT += core gui script scripttools svg

CONFIG += warn_on thread qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mikromosaik
TEMPLATE = app

TRANSLATIONS = mikromosaik_de.ts

CODECFORTR = UTF-8

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    painterpathclass.cpp \
    jsedit/jsedit.cpp \
    random/rnd.cpp \
    random/mersenne_twister.cpp \
    transformclass.cpp \
    colorclass.cpp \
    imageclass.cpp \
    brushclass.cpp \
    scriptrunner.cpp \
    stroke.cpp \
    kineticscroller.cpp \
    util.cpp \
    pointclass.cpp \
    sizeclass.cpp \
    radialgradientclass.cpp \
    gradientclass.cpp

HEADERS += \
    mainwindow.h \
    main.h \
    painterpathclass.h \
    jsedit/jsedit.h \
    random/rnd.h \
    random/mersenne_twister.h \
    random/abstract_random_number_generator.h \
    stroke.h \
    transformclass.h \
    colorclass.h \
    imageclass.h \
    brushclass.h \
    scriptrunner.h \
    kineticscroller.h \
    util.h \
    pointclass.h \
    sizeclass.h \
    gradientclass.h \
    radialgradientclass.h

FORMS += mainwindow.ui

RESOURCES += \
    scripts.qrc \
    images.qrc \
    translations.qrc

OTHER_FILES += \
    scripts/default.js \
    scripts/triangles.js \
    scripts/rotate.js \
    scripts/shear.js \
    scripts/randomly-colored-and-rotated-rectangles.js \
    scripts/greenish-rounded-rectangles.js \
    scripts/red-crosses.js \
    scripts/green-crosses-with-gradient.js \
    scripts/imagebrush.js \
    scripts/red-cross.js \
    scripts/randomly-colored-and-rotated-dark-rectangles.js \
    scripts/gradient.js
