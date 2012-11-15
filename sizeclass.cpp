// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "sizeclass.h"


/*******************************************
 *
 * SizeClass
 *
 *******************************************/

SizeClass::SizeClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QSizeF>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new SizePrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    const QScriptValue& global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


SizeClass::~SizeClass()
{
    /* ... */
}


QString SizeClass::name(void) const
{
    return QLatin1String("Size");
}


QScriptValue SizeClass::prototype(void) const
{
    return mProto;
}


QScriptValue SizeClass::constructor(void)
{
    return mCtor;
}


QScriptValue SizeClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    SizeClass* cls = qscriptvalue_cast<SizeClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    switch (ctx->argumentCount()) {
    case 0: // QSizeF()
        return cls->newInstance();
    case 1: // QSizeF(const QSizeF&)
    {
        const QScriptValue& arg = ctx->argument(0);
        if (arg.instanceOf(ctx->callee())) // QSizeF(const QSizeF&)
            return cls->newInstance(qscriptvalue_cast<QSizeF>(arg));
        return QScriptValue::UndefinedValue;
        break;
    }
    case 2: // QSizeF(qreal w, qreal h)
    {
        const QScriptValue& w = ctx->argument(0);
        const QScriptValue& h = ctx->argument(1);
        return cls->newInstance(QSizeF(w.toNumber(), h.toNumber()));
    }
    }
    return cls->newInstance();
}


QScriptValue SizeClass::toScriptValue(QScriptEngine* eng, const QSizeF& s)
{
    const QScriptValue& ctor = eng->globalObject().property("Size");
    SizeClass* cls = qscriptvalue_cast<SizeClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(s));
    return cls->newInstance(s);
}


void SizeClass::fromScriptValue(const QScriptValue& obj, QSizeF& s)
{
    s = qvariant_cast<QSizeF>(obj.data().toVariant());
}


QScriptValue SizeClass::newInstance(void)
{
    return newInstance(QSizeF());
}


QScriptValue SizeClass::newInstance(const QSizeF& s)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(s));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * SizePrototype
 *
 *******************************************/


SizePrototype::SizePrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


SizePrototype::~SizePrototype()
{
    /* ... */
}


void SizePrototype::setWidth(qreal w)
{
    thisSize()->setWidth(w);
}


void SizePrototype::setHeight(qreal h)
{
    thisSize()->setHeight(h);
}

bool SizePrototype::isEmpty(void) const
{
    return thisSize()->isEmpty();
}


bool SizePrototype::isNull(void) const
{
    return thisSize()->isNull();
}


bool SizePrototype::isValid(void) const
{
    return thisSize()->isValid();
}


qreal SizePrototype::width(void) const
{
    return thisSize()->width();
}


qreal SizePrototype::height(void) const
{
    return thisSize()->width();
}


void SizePrototype::transpose(void)
{
    thisSize()->transpose();
}


QSizeF SizePrototype::boundedTo(const QSizeF & otherSize) const
{
    return thisSize()->boundedTo(otherSize);
}


QSizeF SizePrototype::expandedTo(const QSizeF & otherSize) const
{
    return thisSize()->expandedTo(otherSize);
}


QSizeF* SizePrototype::thisSize(void) const
{
    return qscriptvalue_cast<QSizeF*>(thisObject().data());
}

