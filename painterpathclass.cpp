// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "painterpathclass.h"


/*******************************************
 *
 * PainterPathClass
 *
 *******************************************/

PainterPathClass::PainterPathClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QPainterPath>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new PainterPathPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    const QScriptValue& global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


PainterPathClass::~PainterPathClass()
{
    /* ... */
}


QString PainterPathClass::name(void) const
{
    return QLatin1String("PainterPath");
}


QScriptValue PainterPathClass::prototype(void) const
{
    return mProto;
}


QScriptValue PainterPathClass::constructor(void)
{
    return mCtor;
}


QScriptValue PainterPathClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    PainterPathClass* cls = qscriptvalue_cast<PainterPathClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    const QScriptValue& arg = ctx->argument(0);
    if (arg.instanceOf(ctx->callee()))
        return cls->newInstance(qscriptvalue_cast<QPainterPath>(arg));
    return cls->newInstance();
}


QScriptValue PainterPathClass::toScriptValue(QScriptEngine* eng, const QPainterPath& pp)
{
    const QScriptValue& ctor = eng->globalObject().property("PainterPath");
    PainterPathClass* cls = qscriptvalue_cast<PainterPathClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(pp));
    return cls->newInstance(pp);
}


void PainterPathClass::fromScriptValue(const QScriptValue& obj, QPainterPath& pp)
{
    pp = qvariant_cast<QPainterPath>(obj.data().toVariant());
}


QScriptValue PainterPathClass::newInstance(void)
{
    return newInstance(QPainterPath());
}


QScriptValue PainterPathClass::newInstance(const QPainterPath &pp)
{
    const QScriptValue& data = engine()->newVariant(QVariant::fromValue(pp));
    return engine()->newObject(this, data);
}


/*******************************************
 *
 * PainterPathPrototype
 *
 *******************************************/

PainterPathPrototype::PainterPathPrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


PainterPathPrototype::~PainterPathPrototype()
{
    /* ... */
}


void PainterPathPrototype::addEllipse(qreal x, qreal y, qreal width, qreal height)
{
    thisPainterPath()->addEllipse(x, y, width, height);
}


void PainterPathPrototype::addRect(qreal x, qreal y, qreal width, qreal height)
{
    thisPainterPath()->addRect(x, y, width, height);
}


void PainterPathPrototype::addRoundedRect(qreal x, qreal y, qreal w, qreal h, qreal xRadius, qreal yRadius)
{
    thisPainterPath()->addRoundedRect(x, y, w, h, xRadius, yRadius);
}

void PainterPathPrototype::addText(qreal x, qreal y, const QFont& font, const QString& text)
{
    thisPainterPath()->addText(x, y, font, text);
}


void PainterPathPrototype::moveTo(qreal x, qreal y)
{
    thisPainterPath()->moveTo(x, y);
}


void PainterPathPrototype::lineTo(qreal x, qreal y)
{
    thisPainterPath()->lineTo(x, y);
}


void PainterPathPrototype::quadTo(qreal cx, qreal cy, qreal endPointX, qreal endPointY)
{
    thisPainterPath()->quadTo(cx, cy, endPointX, endPointY);
}


void PainterPathPrototype::cubicTo(qreal c1X, qreal c1Y, qreal c2X, qreal c2Y, qreal endPointX, qreal endPointY)
{
    thisPainterPath()->cubicTo(c1X, c1Y, c2X, c2Y, endPointX, endPointY);
}


qreal PainterPathPrototype::length(void) const
{
    return thisPainterPath()->length();
}


QPainterPath PainterPathPrototype::subtracted(const QPainterPath& p) const
{
    return thisPainterPath()->subtracted(p);
}


QPainterPath* PainterPathPrototype::thisPainterPath() const
{
    return qscriptvalue_cast<QPainterPath*>(thisObject().data());
}
