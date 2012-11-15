// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "util.h"
#include <qmath.h>


QRgb rgbFromWaveLength(qreal wave)
{
    qreal r = 0, g = 0, b = 0;
    if (wave >= 380 && wave <= 440) {
        r = -(wave - 440) / (440 - 380);
        b = 1;
    }
    else if (wave >= 440 && wave <= 490) {
        g = (wave - 440) / (490 - 440);
        b = 1;
    }
    else if (wave >= 490 && wave <= 510) {
        g = 1;
        b = -(wave - 510) / (510 - 490);
    }
    else if (wave >= 510 && wave <= 580) {
        r = (wave - 510) / (580 - 510);
        g = 1;
    }
    else if (wave >= 580 && wave <= 645) {
        r = 1;
        g = -(wave - 645) / (645 - 580);
    }
    else if (wave >= 645 && wave <= 780) {
        r = 1;
    }
    qreal s = 1;
    if (wave > 700)
        s = 0.3 + 0.7 * (780 - wave) / (780 - 700);
    else if (wave <  420)
        s = 0.3 + 0.7 * (wave - 380) / (420 - 380);
    r = qPow(r * s, 0.8);
    g = qPow(g * s, 0.8);
    b = qPow(b * s, 0.8);
    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}


QScriptValue rgbFromWaveLength(QScriptContext* context, QScriptEngine* engine)
{
    if (context->argumentCount() != 1)
        return QScriptValue::UndefinedValue;
    QRgb rgb = rgbFromWaveLength(context->argument(0).toNumber());
    QScriptValue result = engine->newArray(3);
    result.setProperty(0, qRed(rgb));
    result.setProperty(1, qGreen(rgb));
    result.setProperty(2, qBlue(rgb));
    return result;
}
