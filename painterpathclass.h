// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __PAINTERPATHCLASS_H_
#define __PAINTERPATHCLASS_H_

#include <QPainterPath>
#include <QString>
#include <QFont>
#include <QtCore/QObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class PainterPathClass : public QObject, public QScriptClass
{
public:
    PainterPathClass(QScriptEngine* engine = NULL);
    ~PainterPathClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(const QPainterPath&);
    QString name(void) const;
    QScriptValue prototype(void) const;

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("PainterPath", (new PainterPathClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QPainterPath& pp);
    static void fromScriptValue(const QScriptValue& obj, QPainterPath& pp);

    QScriptValue mProto;
    QScriptValue mCtor;
};



class PainterPathPrototype : public QObject, public QScriptable
{
    Q_OBJECT

public:
    PainterPathPrototype(QObject* parent = NULL);
    ~PainterPathPrototype();

public slots:
    void addEllipse(qreal x, qreal y, qreal width, qreal height);
    void addRect(qreal x, qreal y, qreal width, qreal height);
    void addRoundedRect(qreal x, qreal y, qreal w, qreal h, qreal xRadius, qreal yRadius);
    void addText(qreal x, qreal y, const QFont& font, const QString& text);
    void moveTo(qreal x, qreal y);
    void lineTo(qreal x, qreal y);
    void quadTo(qreal cx, qreal cy, qreal endPointX, qreal endPointY);
    void cubicTo(qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY);
    qreal length(void) const;
    QPainterPath subtracted(const QPainterPath&) const;

private:
    QPainterPath* thisPainterPath(void) const;
};



Q_DECLARE_METATYPE(QPainterPath*)
Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(PainterPathClass*)


#endif // __PAINTERPATHCLASS_H_
