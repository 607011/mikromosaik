// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __TRANSFORMCLASS_H_
#define __TRANSFORMCLASS_H_

#include <QtCore/QObject>
#include <QTransform>
#include <QPainterPath>
#include <QTransform>
#include <QtScript/QScriptable>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>

class TransformClass : public QObject, public QScriptClass
{
    Q_OBJECT
public:
    TransformClass(QScriptEngine* engine = NULL);
    ~TransformClass();

    QScriptValue constructor(void);
    QScriptValue newInstance(void);
    QScriptValue newInstance(const QTransform&);
    QString name(void) const;
    QScriptValue prototype(void) const;

    static void Init(QScriptEngine* engine)
    {
        engine->globalObject().setProperty("Transform", (new TransformClass(engine))->constructor());
    }

private:
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QTransform& m);
    static void fromScriptValue(const QScriptValue& obj, QTransform& m);

    QScriptValue mProto;
    QScriptValue mCtor;
};



class TransformPrototype : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(qreal m11 READ m11)
    Q_PROPERTY(qreal m12 READ m12)
    Q_PROPERTY(qreal m13 READ m13)
    Q_PROPERTY(qreal m21 READ m21)
    Q_PROPERTY(qreal m22 READ m22)
    Q_PROPERTY(qreal m23 READ m23)
    Q_PROPERTY(qreal dx READ dx)
    Q_PROPERTY(qreal dy READ dy)
    Q_PROPERTY(qreal D READ determinant)

public:
    TransformPrototype(QObject* parent = NULL);
    ~TransformPrototype();

public slots:
    void setMatrix(qreal m11, qreal m12, qreal m13, qreal m21, qreal m22, qreal m23, qreal m31, qreal m32, qreal m33);
    qreal m11(void) const;
    qreal m12(void) const;
    qreal m13(void) const;
    qreal m21(void) const;
    qreal m22(void) const;
    qreal m23(void) const;
    qreal m31(void) const;
    qreal m32(void) const;
    qreal m33(void) const;
    QTransform adjoint(void) const;
    qreal determinant(void) const;
    qreal dx(void) const;
    qreal dy(void) const;
    QTransform inverted(bool* invertible = NULL) const;
    bool isAffine(void) const;
    bool isIdentity(void) const;
    bool isInvertible(void) const;
    bool isRotating(void) const;
    bool isScaling(void) const;
    bool isTranslating(void) const;
    QPainterPath map(const QPainterPath& path) const;
    QTransform& rotate(qreal angle);
    QTransform& rotateRadians(qreal angle);
    QTransform& shear(qreal sh, qreal sv);
    QTransform& scale(qreal sx, qreal sy);
    QTransform& translate(qreal dx, qreal dy);
    void reset(void);

private:
    QTransform* thisTx(void) const;
};

Q_DECLARE_METATYPE(QTransform*)
Q_DECLARE_METATYPE(QTransform)
Q_DECLARE_METATYPE(TransformClass*)


#endif // __TRANSFORMCLASS_H_
