// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __RADIALGRADIENTCLASS_H_
#define __RADIALGRADIENTCLASS_H_

#include <QtCore/QObject>
#include <QRadialGradient>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>
#include "gradientclass.h"

class RadialGradientClass : public GradientClass
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)

public:
    RadialGradientClass(QScriptEngine* engine = NULL);
    ~RadialGradientClass() { /* ... */ }

    QString name(void) const { return QLatin1String("RadialGradient"); }
    QScriptValue prototype(void) const { return mProto; }
    QScriptValue constructor(void) { return mCtor; }
    QScriptValue newInstance(void) { return newInstance(QRadialGradient()); }
    QScriptValue newInstance(const QRadialGradient&);

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("RadialGradient", (new RadialGradientClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QRadialGradient&);
    static void fromScriptValue(const QScriptValue& obj, QRadialGradient&);

    QScriptValue mProto;
    QScriptValue mCtor;
};


class RadialGradientPrototype : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QPointF center READ center WRITE setCenter)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius)
    Q_PROPERTY(QPointF focalPoint READ focalPoint WRITE setFocalPoint)
    Q_PROPERTY(qreal focalRadius READ focalRadius WRITE setFocalRadius)
    Q_PROPERTY(qreal centerRadius READ centerRadius WRITE setCenterRadius)

public:
    RadialGradientPrototype(QObject* parent = NULL)
        : QObject(parent)
    { /* ... */ }
    ~RadialGradientPrototype() { /* ... */ }

public slots:
    QPointF center(void) const;
    qreal centerRadius(void) const;
    QPointF focalPoint(void) const;
    qreal focalRadius(void) const;
    qreal radius(void) const;
    void setCenter(const QPointF& center);
    void setCenter(qreal x, qreal y);
    void setCenterRadius(qreal radius);
    void setFocalPoint(const QPointF& focalPoint);
    void setFocalPoint(qreal x, qreal y);
    void setFocalRadius(qreal radius);
    void setRadius(qreal radius);

private:
    QRadialGradient* thisGradient(void) const;
};

Q_DECLARE_METATYPE(QRadialGradient)
Q_DECLARE_METATYPE(QRadialGradient*)
Q_DECLARE_METATYPE(RadialGradientClass*)


#endif // __GRADIENTCLASS_H_
