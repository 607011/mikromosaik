// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __SCRIPTRUNNER_H_
#define __SCRIPTRUNNER_H_

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QVector>
#include <QtScript/QScriptEngine>
#ifndef QT_NO_DEBUG
#include <QScriptEngineDebugger>
#endif

#include "stroke.h"
#include "util.h"


class ScriptRunner : public QThread
{
    Q_OBJECT

public:
    ScriptRunner(void);
    ~ScriptRunner() { /* ... */ }
    void stop(void);
    void resume(void);
    const QScriptEngine& engine(void) const { return mScriptEngine; }
    QScriptEngine& engine(void) { return mScriptEngine; }

public slots:
    void setDrawingWidth(int);
    void setDrawingHeight(int);
    void setTileMode(bool enabled);

signals:
    void tilingProgressed(QVector<Stroke> drawing, int N);
    void backgroundBrushChanged(const QBrush&);
    void debug(QString message);

protected:
    void run(void);

private:
    QScriptEngine mScriptEngine;
#ifndef QT_NO_DEBUG
    QScriptEngineDebugger mDebugger;
#endif
    volatile bool mPrestart;
    volatile bool mStopped;
    volatile bool mTileMode;
    QVector<Stroke> mDrawing;
    int mDrawingWidth;
    int mDrawingHeight;

private: // methods
    bool isColliding(const Stroke& stroke) const;
    void decorateStroke(Stroke& stroke, QScriptValue& getBrush, QScriptValue& getColor, const QScriptValueList& vXYArgs);
    void conditionallyAddStroke(Stroke& stroke, QScriptValue& getBrush, QScriptValue& getColor, qreal x0, qreal y0);
    void conditionallyAddStroke(Stroke& stroke);
};


#endif // __SCRIPTRUNNER_H_
