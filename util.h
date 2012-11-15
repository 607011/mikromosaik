// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __UTIL_H_
#define __UTIL_H_

#include <QRgb>
#include <QScriptValue>
#include <QScriptContext>
#include <QScriptEngine>

extern QRgb rgbFromWaveLength(qreal wave);

extern QScriptValue rgbFromWaveLength(QScriptContext*, QScriptEngine*);

#endif // __UTIL_H_
