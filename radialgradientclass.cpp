// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "radialgradientclass.h"


/*******************************************
 *
 * RadialGradientClass
 *
 *******************************************/

RadialGradientClass::RadialGradientClass(QScriptEngine* engine)
    : GradientClass(engine)
{
    qScriptRegisterMetaType<QRadialGradient>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new RadialGradientPrototype(this));
    engine->setDefaultPrototype(qMetaTypeId<QRadialGradient*>(), mProto);
    const QScriptValue& global = engine->globalObject();
    // http://www.qtcentre.org/threads/47245-QtScript-Prototype-based-inheritance-wrappers-and-setDefaultPrototype()
    mProto.setPrototype(global.property("Gradient").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


QScriptValue RadialGradientClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    RadialGradientClass* cls = qscriptvalue_cast<RadialGradientClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    switch (ctx->argumentCount()) {
    case 0: // QRadialGradient()
        return cls->newInstance();
    case 1: // QRadialGradient(const QRadialGradient&)
    {
        const QScriptValue& arg = ctx->argument(0);
        if (arg.instanceOf(ctx->callee()))
            return cls->newInstance(qscriptvalue_cast<QRadialGradient>(arg));
        return QScriptValue::UndefinedValue;
        break;
    }
    }
    return cls->newInstance();
}


QScriptValue RadialGradientClass::toScriptValue(QScriptEngine* eng, const QRadialGradient& g)
{
    const QScriptValue& ctor = eng->globalObject().property("Point");
    RadialGradientClass* cls = qscriptvalue_cast<RadialGradientClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(g));
    return cls->newInstance(g);
}


void RadialGradientClass::fromScriptValue(const QScriptValue& obj, QRadialGradient& g)
{
    g = qvariant_cast<QRadialGradient>(obj.data().toVariant());
}


QScriptValue RadialGradientClass::newInstance(const QRadialGradient& g)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(g));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * RadialGradientPrototype
 *
 *******************************************/

QPointF RadialGradientPrototype::center(void) const
{
    return thisGradient()->center();
}


qreal RadialGradientPrototype::centerRadius(void) const
{
    return thisGradient()->centerRadius();
}


QPointF RadialGradientPrototype::focalPoint(void) const
{
    return thisGradient()->focalPoint();
}


qreal RadialGradientPrototype::focalRadius(void) const
{
    return thisGradient()->focalRadius();
}


qreal RadialGradientPrototype::radius(void) const
{
    return thisGradient()->radius();
}


void RadialGradientPrototype::setCenter(const QPointF& center)
{
    thisGradient()->setCenter(center);
}


void RadialGradientPrototype::setCenter(qreal x, qreal y)
{
    thisGradient()->setCenter(x, y);
}


void RadialGradientPrototype::setCenterRadius(qreal radius)
{
    thisGradient()->setCenterRadius(radius);
}


void RadialGradientPrototype::setFocalPoint(const QPointF& focalPoint)
{
    thisGradient()->setFocalPoint(focalPoint);
}


void RadialGradientPrototype::setFocalPoint(qreal x, qreal y)
{
    thisGradient()->setFocalPoint(x, y);
}


void RadialGradientPrototype::setFocalRadius(qreal radius)
{
    thisGradient()->setFocalRadius(radius);
}


void RadialGradientPrototype::setRadius(qreal radius)
{
    thisGradient()->setRadius(radius);
}


QRadialGradient* RadialGradientPrototype::thisGradient(void) const
{
    return qscriptvalue_cast<QRadialGradient*>(thisObject().data());
}

