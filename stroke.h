// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __STROKE_H_
#define __STROKE_H_

#include <QtCore>
#include <QColor>
#include <QPainterPath>
#include <QBrush>
#include <QRectF>
#include <QTransform>


class Stroke {
public:
    Stroke(const QBrush& brush = QBrush())
        : mBrush(brush)
        , mTranslated(false)
        , mDx(0)
        , mDy(0)
    { /* ... */ }
    Stroke(const Stroke& o)
        : mPath(o.mPath)
        , mBrush(o.mBrush)
        , mTranslated(o.mTranslated)
        , mDx(o.mDx)
        , mDy(o.mDy)
    { /* ... */ }

    enum Excess {
        NoExcess = 0,
        ExcessLeft = 0x1,
        ExcessRight = 0x2,
        ExcessTop = 0x4,
        ExcessBottom = 0x8
    };

    inline void setBrush(const QBrush& brush) { mBrush = brush; }
    inline void setPath(const QPainterPath& path) { mPath = path; }

    inline QPainterPath& path(void) { return mPath; }
    inline const QPainterPath& path(void) const { return mPath; }
    inline const QBrush& brush(void) const { return mBrush; }
    int boundaryExcess(void) const;
    inline bool isTranslated(void) const { return mTranslated; }
    inline void translate(qreal dx, qreal dy)
    {
        mPath.translate(dx, dy);
        mDx = dx;
        mDy = dy;
        mTranslated = true;
    }
    inline qreal dx(void) const { return mDx; }
    inline qreal dy(void) const { return mDy; }

private:
    QPainterPath mPath;
    QBrush mBrush;
    bool mTranslated;
    qreal mDx;
    qreal mDy;
};

#endif // __STROKE_H_
