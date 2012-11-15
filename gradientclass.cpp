// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "gradientclass.h"


/*******************************************
 *
 * GradientClass
 *
 *******************************************/

GradientClass::GradientClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QGradient>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new GradientPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    engine->setDefaultPrototype(qMetaTypeId<QGradient*>(), mProto);
    const QScriptValue& global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


QScriptValue GradientClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    GradientClass* cls = qscriptvalue_cast<GradientClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    switch (ctx->argumentCount()) {
    case 0: // QGradient()
        return cls->newInstance();
    case 1: // QGradient(const QGradient&)
    {
        const QScriptValue& arg = ctx->argument(0);
        if (arg.instanceOf(ctx->callee()))
            return cls->newInstance(qscriptvalue_cast<QGradient>(arg));
        return QScriptValue::UndefinedValue;
        break;
    }
    }
    return cls->newInstance();
}


QScriptValue GradientClass::toScriptValue(QScriptEngine* eng, const QGradient& g)
{
    const QScriptValue& ctor = eng->globalObject().property("Point");
    GradientClass* cls = qscriptvalue_cast<GradientClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(g));
    return cls->newInstance(g);
}


void GradientClass::fromScriptValue(const QScriptValue& obj, QGradient& g)
{
    g = qvariant_cast<QGradient>(obj.data().toVariant());
}


QScriptValue GradientClass::newInstance(void)
{
    return newInstance(QGradient());
}


QScriptValue GradientClass::newInstance(const QGradient& g)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(g));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * GradientPrototype
 *
 *******************************************/


void GradientPrototype::setColorAt(qreal position, const QColor& color)
{
    thisGradient()->setColorAt(position, color);
}


void GradientPrototype::setCoordinateMode(QGradient::CoordinateMode mode)
{
    thisGradient()->setCoordinateMode(mode);
}


void GradientPrototype::setInterpolationMode(QGradient::InterpolationMode mode)
{
    thisGradient()->setInterpolationMode(mode);
}


void GradientPrototype::setSpread(QGradient::Spread method)
{
    thisGradient()->setSpread(method);
}


QGradient::Spread GradientPrototype::spread(void) const
{
    return thisGradient()->spread();
}


QGradient::Type GradientPrototype::type(void) const
{
    return thisGradient()->type();
}


QGradient::CoordinateMode GradientPrototype::coordinateMode(void) const
{
    return thisGradient()->coordinateMode();
}


QGradient* GradientPrototype::thisGradient(void) const
{
    return qscriptvalue_cast<QGradient*>(thisObject().data());
}

