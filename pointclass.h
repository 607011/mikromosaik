// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __POINTCLASS_H_
#define __POINTCLASS_H_

#include <QtCore/QObject>
#include <QPointF>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class PointClass : public QObject, public QScriptClass
{
public:
    PointClass(QScriptEngine* engine = NULL);
    ~PointClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(qreal x, qreal y);
    QScriptValue newInstance(const QPointF&);
    QString name(void) const;
    QScriptValue prototype(void) const;

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("Point", (new PointClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QPointF&);
    static void fromScriptValue(const QScriptValue& obj, QPointF&);

    QScriptValue mProto;
    QScriptValue mCtor;
};


class PointPrototype : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)

public:
    PointPrototype(QObject* parent = NULL);
    ~PointPrototype();

public slots:
    void setX(qreal x);
    void setY(qreal x);
    qreal x(void) const;
    qreal y(void) const;

private:
    QPointF* thisPoint(void) const;
};



Q_DECLARE_METATYPE(QPointF*)
Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(PointClass*)


#endif // __POINTCLASS_H_
