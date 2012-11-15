// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __SIZECLASS_H_
#define __SIZECLASS_H_

#include <QtCore/QObject>
#include <QSizeF>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class SizeClass : public QObject, public QScriptClass
{
public:
    SizeClass(QScriptEngine* engine = NULL);
    ~SizeClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(qreal w, qreal h);
    QScriptValue newInstance(const QSizeF&);
    QString name(void) const;
    QScriptValue prototype(void) const;

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("Size", (new SizeClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QSizeF&);
    static void fromScriptValue(const QScriptValue& obj, QSizeF&);

    QScriptValue mProto;
    QScriptValue mCtor;
};


class SizePrototype : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(qreal height READ height WRITE setHeight)

public:
    SizePrototype(QObject* parent = NULL);
    ~SizePrototype();

public slots:
    bool isEmpty(void) const;
    bool isNull(void) const;
    bool isValid(void) const;
    void setWidth(qreal w);
    void setHeight(qreal h);
    void transpose(void);
    QSizeF boundedTo(const QSizeF & otherSize) const;
    QSizeF expandedTo(const QSizeF & otherSize) const;
    qreal width(void) const;
    qreal height(void) const;

private:
    QSizeF* thisSize(void) const;
};



Q_DECLARE_METATYPE(QSizeF*)
Q_DECLARE_METATYPE(QSizeF)
Q_DECLARE_METATYPE(SizeClass*)


#endif // __SIZECLASS_H_
