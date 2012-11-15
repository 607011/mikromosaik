// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __BRUSHCLASS_H_
#define __BRUSHCLASS_H_

#include <QBrush>
#include <QTransform>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class BrushClass : public QObject, public QScriptClass
{
public:
    BrushClass(QScriptEngine* engine = NULL);
    ~BrushClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(const QBrush&);
    QString name(void) const;
    QScriptValue prototype(void) const;

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("Brush", (new BrushClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QBrush&);
    static void fromScriptValue(const QScriptValue& obj, QBrush&);

    QScriptValue mProto;
    QScriptValue mCtor;
};




class BrushPrototype : public QObject, public QScriptable
{
    Q_OBJECT

public:
    BrushPrototype(QObject* parent = NULL);
    ~BrushPrototype();

public slots:
    void setTransform(const QTransform&);

private:
    QBrush* thisBrush(void) const;
};


Q_DECLARE_METATYPE(QBrush*)
Q_DECLARE_METATYPE(QBrush)
Q_DECLARE_METATYPE(BrushClass*)



#endif // __BRUSHCLASS_H_
