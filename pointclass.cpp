// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "pointclass.h"


/*******************************************
 *
 * PointClass
 *
 *******************************************/

PointClass::PointClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QPointF>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new PointPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    const QScriptValue& global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


PointClass::~PointClass()
{
    /* ... */
}


QString PointClass::name(void) const
{
    return QLatin1String("Point");
}


QScriptValue PointClass::prototype(void) const
{
    return mProto;
}


QScriptValue PointClass::constructor(void)
{
    return mCtor;
}


QScriptValue PointClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    PointClass* cls = qscriptvalue_cast<PointClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    switch (ctx->argumentCount()) {
    case 0: // QPointF()
        return cls->newInstance();
    case 1: // QPointF(const QPointF&)
    {
        const QScriptValue& arg = ctx->argument(0);
        if (arg.instanceOf(ctx->callee())) // QColor(const QColor&)
            return cls->newInstance(qscriptvalue_cast<QPointF>(arg));
        return QScriptValue::UndefinedValue;
        break;
    }
    case 2: // QPoint(qreal x, qreal y)
    {
        const QScriptValue& x = ctx->argument(0);
        const QScriptValue& y = ctx->argument(1);
        return cls->newInstance(QPointF(x.toNumber(), y.toNumber()));
    }
    }
    return cls->newInstance();
}


QScriptValue PointClass::toScriptValue(QScriptEngine* eng, const QPointF& p)
{
    const QScriptValue& ctor = eng->globalObject().property("Point");
    PointClass* cls = qscriptvalue_cast<PointClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(p));
    return cls->newInstance(p);
}


void PointClass::fromScriptValue(const QScriptValue& obj, QPointF& p)
{
    p = qvariant_cast<QPointF>(obj.data().toVariant());
}


QScriptValue PointClass::newInstance(void)
{
    return newInstance(QPointF());
}


QScriptValue PointClass::newInstance(const QPointF& p)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(p));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * PointPrototype
 *
 *******************************************/


PointPrototype::PointPrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


PointPrototype::~PointPrototype()
{
    /* ... */
}


void PointPrototype::setX(qreal x)
{
    thisPoint()->setX(x);
}


void PointPrototype::setY(qreal y)
{
    thisPoint()->setY(y);
}


qreal PointPrototype::x(void) const
{
    return thisPoint()->x();
}


qreal PointPrototype::y(void) const
{
    return thisPoint()->y();
}


QPointF* PointPrototype::thisPoint(void) const
{
    return qscriptvalue_cast<QPointF*>(thisObject().data());
}

