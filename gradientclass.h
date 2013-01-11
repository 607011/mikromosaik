// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __GRADIENTCLASS_H_
#define __GRADIENTCLASS_H_

#include <QtCore/QObject>
#include <QGradient>
#include <QMetaObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptable>


class GradientClass : public QObject, public QScriptClass
{
    Q_OBJECT

public:
    GradientClass(QScriptEngine* engine);

    enum Type {
        LinearGradient,
        RadialGradient,
        ConicalGradient,
        NoGradient
    };
    Q_ENUMS(Type)

    enum Spread {
        PadSpread,
        ReflectSpread,
        RepeatSpread
    };
    Q_ENUMS(Spread)

    enum CoordinateMode {
        LogicalMode,
        StretchToDeviceMode,
        ObjectBoundingMode
    };
    Q_ENUMS(CoordinateMode)

    enum InterpolationMode {
        ColorInterpolation,
        ComponentInterpolation
    };
    Q_ENUMS(InterpolationMode)

public:
    virtual QString name(void) const { return QLatin1String("Gradient"); }
    QScriptValue GradientClass::prototype(void) const { return mProto; }
    QScriptValue GradientClass::constructor(void) { return mCtor; }

    static QScriptValue toScriptValue(QScriptEngine* engine, const QGradient::Type &s)
    {
        return engine->newVariant((int)s);
    }

    static void fromScriptValue(const QScriptValue& obj, QGradient::Type& s)
    {
        s = (QGradient::Type)obj.toInt32();
    }

    static QScriptValue toScriptValue(QScriptEngine* engine, const QGradient::Spread &s)
    {
        return engine->newVariant((int)s);
    }

    static void fromScriptValue(const QScriptValue& obj, QGradient::Spread& s)
    {
        s = (QGradient::Spread)obj.toInt32();
    }

    static QScriptValue toScriptValue(QScriptEngine* engine, const QGradient::CoordinateMode &s)
    {
        return engine->newVariant((int)s);
    }

    static void fromScriptValue(const QScriptValue& obj, QGradient::CoordinateMode& s)
    {
        s = (QGradient::CoordinateMode)obj.toInt32();
    }

    static QScriptValue toScriptValue(QScriptEngine* engine, const QGradient::InterpolationMode &s)
    {
        return engine->newVariant((int)s);
    }

    static void fromScriptValue(const QScriptValue& obj, QGradient::InterpolationMode& s)
    {
        s = (QGradient::InterpolationMode)obj.toInt32();
    }

    static void Init(QScriptEngine* engine)
    {
        qScriptRegisterMetaType<QGradient::Type>(engine, toScriptValue, fromScriptValue);
        qScriptRegisterMetaType<QGradient::Spread>(engine, toScriptValue, fromScriptValue);
        qScriptRegisterMetaType<QGradient::CoordinateMode>(engine, toScriptValue, fromScriptValue);
        qScriptRegisterMetaType<QGradient::InterpolationMode>(engine, toScriptValue, fromScriptValue);
        const QScriptValue& metaObject = engine->newQMetaObject(&staticMetaObject, engine->newFunction(construct));
        engine->globalObject().setProperty("Gradient", metaObject);
    }

private:
    static QScriptValue enumConstructor(QScriptContext* context, QScriptEngine* engine)
    {
        QObject* object = new QObject(context->argument(0).toQObject());
        return engine->newQObject(object, QScriptEngine::ScriptOwnership);
    }

    QScriptValue newInstance(void);
    QScriptValue newInstance(const QGradient&);
    static QScriptValue construct(QScriptContext* ctx, QScriptEngine* eng);
    static QScriptValue toScriptValue(QScriptEngine* eng, const QGradient&);
    static void fromScriptValue(const QScriptValue& obj, QGradient&);

    QScriptValue mProto;
    QScriptValue mCtor;
};

Q_DECLARE_METATYPE(QGradient::Type)
Q_DECLARE_METATYPE(QGradient::CoordinateMode)
Q_DECLARE_METATYPE(QGradient::InterpolationMode)
Q_DECLARE_METATYPE(QGradient::Spread)
Q_DECLARE_METATYPE(QGradient*)
Q_DECLARE_METATYPE(QGradient)
Q_DECLARE_METATYPE(GradientClass*)


class GradientPrototype : public QObject, public QScriptable
{
    Q_OBJECT
    Q_PROPERTY(QGradient::CoordinateMode coordinateMode READ coordinateMode WRITE setCoordinateMode)
    Q_PROPERTY(QGradient::Spread spread READ spread WRITE setSpread)
    Q_PROPERTY(QGradient::Type type READ type)

public:
    GradientPrototype(QObject* parent = NULL)
        : QObject(parent)
    { /* ... */ }
    ~GradientPrototype() { /* ... */ }

public slots:
    void setColorAt(qreal position, const QColor& color);
    void setCoordinateMode(QGradient::CoordinateMode mode);
    void setInterpolationMode(QGradient::InterpolationMode mode);
    void setSpread(QGradient::Spread method);
    QGradient::CoordinateMode coordinateMode(void) const;
    QGradient::Spread spread(void) const;
    QGradient::Type type(void) const;

private:
    QGradient* thisGradient(void) const;
};

#endif // __GRADIENTCLASS_H_
