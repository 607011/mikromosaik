// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "colorclass.h"


/*******************************************
 *
 * ColorClass
 *
 *******************************************/

ColorClass::ColorClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QColor>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new ColorPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    mProto.setPrototype(engine->globalObject().property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


ColorClass::~ColorClass()
{
    /* ... */
}


QString ColorClass::name(void) const
{
    return QLatin1String("Color");
}


QScriptValue ColorClass::prototype(void) const
{
    return mProto;
}


QScriptValue ColorClass::constructor(void)
{
    return mCtor;
}


QScriptValue ColorClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    ColorClass* cls = qscriptvalue_cast<ColorClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    switch (ctx->argumentCount()) {
    case 0: // QColor()
        return cls->newInstance();
    case 1: // QColor(const QColor&) oder QColor(QRgb)
    {
        const QScriptValue& arg = ctx->argument(0);
        if (arg.instanceOf(ctx->callee())) // QColor(const QColor&)
            return cls->newInstance(qscriptvalue_cast<QColor>(arg));
        if (arg.isString()) {
            return cls->newInstance(QColor(arg.toString())); // QColor(QRgb)
        }
        else if (arg.isNumber()) {
            return cls->newInstance(QColor((QRgb)(arg.toUInt32()))); // QColor(const QString&)
        }
        break;
    }
    case 3: // QColor(int r, int g, int b)
    {
        const QScriptValue& r = ctx->argument(0);
        const QScriptValue& g = ctx->argument(1);
        const QScriptValue& b = ctx->argument(2);
        return cls->newInstance(QColor(r.toInt32(), g.toInt32(), b.toInt32()));
    }
    case 4: // QColor(int r, int g, int b, int a)
    {
        const QScriptValue& r = ctx->argument(0);
        const QScriptValue& g = ctx->argument(1);
        const QScriptValue& b = ctx->argument(2);
        const QScriptValue& a = ctx->argument(3);
        return cls->newInstance(QColor(r.toInt32(), g.toInt32(), b.toInt32(), a.toInt32()));
    }
    }
    return cls->newInstance();
}


QScriptValue ColorClass::toScriptValue(QScriptEngine* eng, const QColor& c)
{
    const QScriptValue& ctor = eng->globalObject().property("Color");
    ColorClass* cls = qscriptvalue_cast<ColorClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(c));
    return cls->newInstance(c);
}


void ColorClass::fromScriptValue(const QScriptValue& obj, QColor& c)
{
    c = qvariant_cast<QColor>(obj.data().toVariant());
}


QScriptValue ColorClass::newInstance(void)
{
    return newInstance(QColor());
}


QScriptValue ColorClass::newInstance(const QColor& c)
{
    const QScriptValue& data = engine()->newVariant(QVariant::fromValue(c));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * ColorPrototype
 *
 *******************************************/


ColorPrototype::ColorPrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


ColorPrototype::~ColorPrototype()
{
    /* ... */
}


void ColorPrototype::setRgb(int r, int g, int b, int a)
{
    thisColor()->setRgb(r, g, b, a);
}


int ColorPrototype::alpha(void) const
{
    return thisColor()->alpha();
}


int ColorPrototype::black(void) const
{
    return thisColor()->black();
}


int ColorPrototype::blue(void) const
{
    return thisColor()->blue();
}


int ColorPrototype::cyan(void) const
{
    return thisColor()->cyan();
}


QColor ColorPrototype::darker(int factor) const
{
    return thisColor()->darker(factor);
}


QColor ColorPrototype::lighter(int factor) const
{
    return thisColor()->lighter(factor);
}


int ColorPrototype::green(void) const
{
    return thisColor()->green();
}


int ColorPrototype::hue(void) const{
    return thisColor()->hue();
}


int ColorPrototype::lightness(void) const
{
    return thisColor()->lightness();
}


int ColorPrototype::magenta(void) const
{
    return thisColor()->magenta();
}


int ColorPrototype::yellow(void) const
{
    return thisColor()->yellow();
}


QString ColorPrototype::name(void) const
{
    return thisColor()->name();
}


int ColorPrototype::red(void) const
{
    return thisColor()->red();
}


int ColorPrototype::saturation(void) const
{
    return thisColor()->saturation();
}


QColor* ColorPrototype::thisColor() const
{
    return qscriptvalue_cast<QColor*>(thisObject().data());
}

