// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include <QtCore/QDebug>
#include "brushclass.h"
#include "gradientclass.h"

/*******************************************
 *
 * BrushClass
 *
 *******************************************/

BrushClass::BrushClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QBrush>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new BrushPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    const QScriptValue& global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


BrushClass::~BrushClass()
{
    /* ... */
}


QString BrushClass::name(void) const
{
    return QLatin1String("Brush");
}


QScriptValue BrushClass::prototype(void) const
{
    return mProto;
}


QScriptValue BrushClass::constructor(void)
{
    return mCtor;
}


QScriptValue BrushClass::construct(QScriptContext* ctx, QScriptEngine* engine)
{
    BrushClass* cls = qscriptvalue_cast<BrushClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    switch (ctx->argumentCount()) {
    case 0: // QBrush()
        return cls->newInstance();
    case 1:
    {
        const QScriptValue& arg = ctx->argument(0);
        if (arg.instanceOf(ctx->callee())) {
            // QBrush(const QBrush&)
            qDebug() << "QBrush(const QBrush&)";
            return cls->newInstance(qscriptvalue_cast<QBrush>(arg));
        }
        else if (arg.instanceOf(qScriptValueFromValue(engine, QImage()))) {
            // QBrush(const QImage&)
            qDebug() << "QBrush(const QImage&)";
            return cls->newInstance(qscriptvalue_cast<QImage>(arg));
        }
        break;
    }
    case 3: // QBrush(int r, int g, int b)
    {
        const int r = ctx->argument(0).toInt32();
        const int g = ctx->argument(1).toInt32();
        const int b = ctx->argument(2).toInt32();
        return cls->newInstance(QBrush(QColor(r, g, b)));
    }
    }
    return cls->newInstance();
}


QScriptValue BrushClass::toScriptValue(QScriptEngine* eng, const QBrush& brush)
{
    const QScriptValue& ctor = eng->globalObject().property("Brush");
    BrushClass* cls = qscriptvalue_cast<BrushClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(brush));
    return cls->newInstance(brush);
}


void BrushClass::fromScriptValue(const QScriptValue& obj, QBrush& brush)
{
    brush = qvariant_cast<QBrush>(obj.data().toVariant());
}


QScriptValue BrushClass::newInstance(void)
{
    return newInstance(QBrush());
}


QScriptValue BrushClass::newInstance(const QBrush& brush)
{
    QScriptValue data = engine()->newVariant(QVariant::fromValue(brush));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * BrushPrototype
 *
 *******************************************/

BrushPrototype::BrushPrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


BrushPrototype::~BrushPrototype()
{
    /* ... */
}


void BrushPrototype::setTransform(const QTransform& transform)
{
    thisBrush()->setTransform(transform);
}


QBrush* BrushPrototype::thisBrush() const
{
    return qscriptvalue_cast<QBrush*>(thisObject().data());
}
