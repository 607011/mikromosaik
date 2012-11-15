// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __COLORCLASS_H_
#define __COLORCLASS_H_

#include <QtCore/QObject>
#include <QColor>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class ColorClass : public QObject, public QScriptClass
{
public:
    ColorClass(QScriptEngine* engine = NULL);
    ~ColorClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(const QColor&);
    QString name(void) const;
    QScriptValue prototype(void) const;

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("Color", (new ColorClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QColor&);
    static void fromScriptValue(const QScriptValue& obj, QColor&);

    QScriptValue mProto;
    QScriptValue mCtor;
};


class ColorPrototype : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(int r READ red)
    Q_PROPERTY(int g READ green)
    Q_PROPERTY(int b READ blue)
    Q_PROPERTY(int a READ alpha)
    Q_PROPERTY(int c READ cyan)
    Q_PROPERTY(int m READ magenta)
    Q_PROPERTY(int y READ yellow)
    Q_PROPERTY(int k READ black)
    Q_PROPERTY(int h READ hue)
    Q_PROPERTY(int s READ saturation)
    Q_PROPERTY(int l READ lightness)

public:
    ColorPrototype(QObject* parent = NULL);
    ~ColorPrototype();

public slots:
    void setRgb(int r, int g, int b, int a = 255);
    int alpha(void) const;
    int black(void) const;
    int blue(void) const;
    int cyan(void) const;
    QColor darker(int factor = 200) const;
    int green(void) const;
    int hue(void) const;
    QColor lighter(int factor = 150) const;
    int lightness(void) const;
    int magenta(void) const;
    int yellow(void) const;
    QString name(void) const;
    int red(void) const;
    int saturation(void) const;

private:
    QColor* thisColor(void) const;
};


Q_DECLARE_METATYPE(QColor*)
Q_DECLARE_METATYPE(QColor)
Q_DECLARE_METATYPE(ColorClass*)


#endif // __COLORCLASS_H_
