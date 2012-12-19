// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtCore/QDebug>

#include "random/rnd.h"
#include "scriptrunner.h"
#include "pointclass.h"
#include "sizeclass.h"
#include "painterpathclass.h"
#include "transformclass.h"
#include "colorclass.h"
#include "imageclass.h"
#include "brushclass.h"
#include "gradientclass.h"
#include "radialgradientclass.h"


ScriptRunner::ScriptRunner(void)
    : mStopped(false)
    , mPrestart(true)
    , mTileMode(true)
    , mDrawingWidth(0)
    , mDrawingHeight(0)
{
    RAND::initialize();

    // set up scripting engine
    mScriptEngine.globalObject().setProperty("rgbFromWaveLength", mScriptEngine.newFunction(rgbFromWaveLength, 1));
    PointClass::Init(&mScriptEngine);
    SizeClass::Init(&mScriptEngine);
    PainterPathClass::Init(&mScriptEngine);
    TransformClass::Init(&mScriptEngine);
    ColorClass::Init(&mScriptEngine);
    ImageClass::Init(&mScriptEngine);
    BrushClass::Init(&mScriptEngine);
    GradientClass::Init(&mScriptEngine);
    RadialGradientClass::Init(&mScriptEngine);

#ifndef QT_NO_DEBUG
    //  mDebugger.attachTo(&mScriptEngine);
    //  mDebugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
    //  mDebugger.widget(QScriptEngineDebugger::DebugOutputWidget)->show();
#endif

    // make sure that Qt's threading system is up and running to prevent lagged thread execution after pressing "run" the first time
    start();
}


void ScriptRunner::stop(void)
{
    mStopped = true;
}


void ScriptRunner::resume(void)
{
    mStopped = false;
    start();
}


void ScriptRunner::setDrawingWidth(int w)
{
    mDrawingWidth = w;
}


void ScriptRunner::setDrawingHeight(int h)
{
    mDrawingHeight = h;
}


void ScriptRunner::setTileMode(bool enabled)
{
    mTileMode = enabled;
}


static const QBrush DefaultBrush[3] =
{
    QBrush(QColor(255, 0, 0, 200)),
    QBrush(QColor(255, 255, 0, 200)),
    QBrush(QColor(0, 255, 0, 200)),
};


void ScriptRunner::decorateStroke(Stroke& stroke, QScriptValue& getBrush, QScriptValue& getColor, const QScriptValueList& vXYArgs)
{
    if (getBrush.isFunction()) {
        const QScriptValue& vBrush = getBrush.call(QScriptValue(), vXYArgs);
        stroke.setBrush(qscriptvalue_cast<QBrush>(vBrush));
    }
    else if (getColor.isFunction()) {
        const QScriptValue& vColor = getColor.call(QScriptValue(), vXYArgs);
        const QColor& color = qscriptvalue_cast<QColor>(vColor);
        if (color.isValid())
            stroke.setBrush(QBrush(color));
    }
}


inline void ScriptRunner::conditionallyAddStroke(Stroke& stroke, QScriptValue& getBrush, QScriptValue& getColor, qreal x0, qreal y0)
{
    if (stroke.isTranslated()) {
        QScriptValueList vXYArgs;
        vXYArgs << x0 + stroke.dx() << y0 + stroke.dy();
        decorateStroke(stroke, getBrush, getColor, vXYArgs);
        mDrawing.append(stroke);
    }
}


inline void ScriptRunner::conditionallyAddStroke(Stroke& stroke)
{
    if (stroke.isTranslated()) {
        mDrawing.append(stroke);
    }
}


void ScriptRunner::run(void)
{
    if (mPrestart) {
        mPrestart = false;
        return;
    }
    QScriptValue& globals = mScriptEngine.globalObject();
    QScriptValue& vN = globals.property("N");
    if (!vN.isNumber()) {
        const int DefaultN = 2500;
        emit debug(tr("`N` is not a number. Using default (%1).").arg(DefaultN));
        globals.setProperty("N", DefaultN);
    }
    const int N = vN.toInt32();
    QScriptValue& vMaxTrials = globals.property("MAX_TRIALS");
    if (!vMaxTrials.isNumber()) {
        const int DefaultMaxTrials = 3000;
        emit debug(tr("`MAX_TRIALS` is not a number. Using default (%1).").arg(DefaultMaxTrials));
        globals.setProperty("MAX_TRIALS", DefaultMaxTrials);
    }
    const int MAX_TRIALS = vMaxTrials.toInt32();
    QScriptValue& proceed = globals.property("proceed");
    if (!proceed.isFunction()) {
        emit debug(tr("`proceed()` is not defined. Doing nothing."));
        return;
    }
    QScriptValue& getShape = globals.property("getShape");
    if (!getShape.isFunction()) {
        emit debug(tr("`getShape()` is not defined. Using defaults."));
    }
    QScriptValue& getColor = globals.property("getColor");
    if (!getColor.isFunction()) {
        emit debug(tr("`getColor()` is not defined. Using defaults."));
    }
    QScriptValue& getBrush = globals.property("getBrush");
    if (!getBrush.isFunction()) {
        emit debug(tr("`getBrush()` is not defined. Using defaults."));
    }
    QScriptValue& getBackgroundBrush = globals.property("getBackgroundBrush");
    if (!getBackgroundBrush.isFunction()) {
        emit debug(tr("`getBackgroundBrush()` is not defined. Using defaults."));
    }
    else {
        const QScriptValue& vBrush = getBackgroundBrush.call();
        if (vBrush.isObject())
            emit backgroundBrushChanged(qscriptvalue_cast<QBrush>(vBrush));
    }
    globals.setProperty("S", QScriptValue::UndefinedValue);
    globals.setProperty("W", mDrawingWidth);
    globals.setProperty("H", mDrawingHeight);
    mDrawing.clear();
    int numElements = 0;
    while (!mStopped && numElements < N) {
        globals.setProperty("i", numElements);
        proceed.call();
        int numTrials = 0;
        bool colliding = true;
        while (!mStopped && colliding && numTrials < MAX_TRIALS) {
            ++numTrials;
            Stroke stroke(DefaultBrush[numElements % 3]);
            const qreal x0 = RAND::rnd1();
            const qreal y0 = RAND::rnd1();
            QScriptValueList vXYArgs;
            vXYArgs << x0 << y0;
            if (getShape.isFunction()) {
                const QScriptValue& vShape = getShape.call(QScriptValue(), vXYArgs);
                stroke.setPath(qscriptvalue_cast<QPainterPath>(vShape));
            }
            else {
                const qreal S = globals.property("S").toNumber() / 2;
                stroke.path().addRect(x0-S, y0-S, S, S);
            }
            colliding = isColliding(stroke);
            if (!colliding) {
                if (!mTileMode) {
                    decorateStroke(stroke, getBrush, getColor, vXYArgs);
                    mDrawing.append(stroke);
                    ++numElements;
                }
                else {
                    decorateStroke(stroke, getBrush, getColor, vXYArgs);
                    const int excess = stroke.boundaryExcess();
                    Stroke stroke2 = stroke;
                    if (excess & Stroke::ExcessLeft)
                        stroke2.translate(1, 0);
                    else if (excess & Stroke::ExcessRight)
                        stroke2.translate(-1, 0);
                    Stroke stroke3 = stroke;
                    if (excess & Stroke::ExcessTop)
                        stroke3.translate(0, 1);
                    else if (excess & Stroke::ExcessBottom)
                        stroke3.translate(0, -1);
                    Stroke stroke4 = stroke;
                    if (stroke2.isTranslated() && stroke3.isTranslated())
                        stroke4.translate(stroke2.dx(), stroke3.dy());
                    colliding =
                            (stroke2.isTranslated() && isColliding(stroke2)) ||
                            (stroke3.isTranslated() && isColliding(stroke3)) ||
                            (stroke4.isTranslated() && isColliding(stroke4));
                    if (!colliding) {
                        mDrawing.append(stroke);
                        if (getBrush.isFunction()) {
                            conditionallyAddStroke(stroke2, getBrush, getColor, x0, y0);
                            conditionallyAddStroke(stroke3, getBrush, getColor, x0, y0);
                            conditionallyAddStroke(stroke4, getBrush, getColor, x0, y0);
                        }
                        else {
                            conditionallyAddStroke(stroke2);
                            conditionallyAddStroke(stroke3);
                            conditionallyAddStroke(stroke4);
                        }
                        ++numElements;
                    }
                }
            }
        }
        emit tilingProgressed(mDrawing, N);
    }
}


inline bool ScriptRunner::isColliding(const Stroke& stroke) const
{
    for (QVector<Stroke>::const_iterator it = mDrawing.constBegin(); it != mDrawing.constEnd(); ++it)
        if (it->path().intersects(stroke.path()))
            return true;
    return false;
}
