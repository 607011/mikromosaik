// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtScript/QScriptEngine>
#include "transformclass.h"



/*******************************************
 *
 * TransformClass
 *
 *******************************************/


TransformClass::TransformClass(QScriptEngine* engine)
    : QObject(engine)
    , QScriptClass(engine)
{
    qScriptRegisterMetaType<QTransform>(engine, toScriptValue, fromScriptValue);
    mProto = engine->newQObject(new TransformPrototype(this),
                                QScriptEngine::QtOwnership,
                                QScriptEngine::SkipMethodsInEnumeration
                                | QScriptEngine::ExcludeSuperClassMethods
                                | QScriptEngine::ExcludeSuperClassProperties);
    const QScriptValue& global = engine->globalObject();
    mProto.setPrototype(global.property("Object").property("prototype"));
    mCtor = engine->newFunction(construct, mProto);
    mCtor.setData(engine->toScriptValue(this));
}


TransformClass::~TransformClass()
{
    /* ... */
}


QString TransformClass::name(void) const
{
    return QLatin1String("Transform");
}


QScriptValue TransformClass::prototype(void) const
{
    return mProto;
}


QScriptValue TransformClass::constructor(void)
{
    return mCtor;
}


QScriptValue TransformClass::construct(QScriptContext* ctx, QScriptEngine*)
{
    TransformClass* cls = qscriptvalue_cast<TransformClass*>(ctx->callee().data());
    if (!cls)
        return QScriptValue();
    const QScriptValue& arg = ctx->argument(0);
    if (arg.instanceOf(ctx->callee()))
        return cls->newInstance(qscriptvalue_cast<QTransform>(arg));
    return cls->newInstance();
}


QScriptValue TransformClass::toScriptValue(QScriptEngine* eng, const QTransform& m)
{
    const QScriptValue& ctor = eng->globalObject().property("Transform");
    TransformClass* cls = qscriptvalue_cast<TransformClass*>(ctor.data());
    if (!cls)
        return eng->newVariant(QVariant::fromValue(m));
    return cls->newInstance(m);
}


void TransformClass::fromScriptValue(const QScriptValue& obj, QTransform& m)
{
    m = qvariant_cast<QTransform>(obj.data().toVariant());
}


QScriptValue TransformClass::newInstance(void)
{
    return newInstance(QTransform());
}


QScriptValue TransformClass::newInstance(const QTransform &m)
{
    const QScriptValue& data = engine()->newVariant(QVariant::fromValue(m));
    return engine()->newObject(this, data);
}



/*******************************************
 *
 * TransformPrototype
 *
 *******************************************/

TransformPrototype::TransformPrototype(QObject* parent)
    : QObject(parent)
{
    /* ... */
}


TransformPrototype::~TransformPrototype()
{
    /* ... */
}


QTransform* TransformPrototype::thisTx(void) const
{
    return qscriptvalue_cast<QTransform*>(thisObject().data());
}


qreal TransformPrototype::m11(void) const
{
    return thisTx()->m11();
}


qreal TransformPrototype::m12(void) const
{
    return thisTx()->m12();
}


qreal TransformPrototype::m13(void) const
{
    return thisTx()->m13();
}


qreal TransformPrototype::m21(void) const
{
    return thisTx()->m21();
}


qreal TransformPrototype::m22(void) const
{
    return thisTx()->m22();
}


qreal TransformPrototype::m23(void) const
{
    return thisTx()->m23();
}


qreal TransformPrototype::m31() const
{
    return thisTx()->m31();
}


qreal TransformPrototype::m32(void) const
{
    return thisTx()->m32();
}


qreal TransformPrototype::m33(void) const
{
    return thisTx()->m33();
}


QTransform TransformPrototype::adjoint(void) const
{
    return thisTx()->adjoint();
}


qreal TransformPrototype::determinant(void) const
{
    return thisTx()->determinant();
}


qreal TransformPrototype::dx(void) const
{
    return thisTx()->dx();
}


qreal TransformPrototype::dy(void) const
{
    return thisTx()->dy();
}


QTransform TransformPrototype::inverted(bool* invertible) const
{
    return thisTx()->inverted(invertible);
}


bool TransformPrototype::isAffine(void) const
{
    return thisTx()->isAffine();
}


bool TransformPrototype::isIdentity(void) const
{
    return thisTx()->isIdentity();
}


bool TransformPrototype::isInvertible(void) const
{
    return thisTx()->isInvertible();
}


bool TransformPrototype::isRotating(void) const
{
    return thisTx()->isRotating();
}


bool TransformPrototype::isScaling(void) const
{
    return thisTx()->isScaling();
}


bool TransformPrototype::isTranslating(void) const
{
    return thisTx()->isTranslating();
}


QPainterPath TransformPrototype::map(const QPainterPath &path) const
{
    return thisTx()->map(path);
}


QTransform& TransformPrototype::rotate(qreal angle)
{
    return thisTx()->rotate(angle);
}


QTransform& TransformPrototype::rotateRadians(qreal angle)
{
    return thisTx()->rotateRadians(angle);
}


QTransform& TransformPrototype::shear(qreal sh, qreal sv)
{
    return thisTx()->shear(sh, sv);
}


QTransform& TransformPrototype::scale(qreal sx, qreal sy)
{
    return thisTx()->scale(sx, sy);
}


QTransform& TransformPrototype::translate(qreal dx, qreal dy)
{
    return thisTx()->translate(dx, dy);
}


void TransformPrototype::reset(void)
{
    thisTx()->reset();
}


void TransformPrototype::setMatrix(qreal m11, qreal m12, qreal m13, qreal m21, qreal m22, qreal m23, qreal m31, qreal m32, qreal m33)
{
    thisTx()->setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
}
