// Copyright (c) 2011-2012 Oliver Lau <ola@ct.de>
// All rights reserved.

#ifndef __KINETICSCROLLER_H_
#define __KINETICSCROLLER_H_

#include <QObject>
#include <QEvent>
#include <QVector>
#include <QTime>
#include <QWidget>
#include <QPoint>
#include <QTimerEvent>

class KineticScroller : public QObject
{
    Q_OBJECT
public:
    KineticScroller(QWidget* scrollArea = NULL, QObject* parent = NULL);
    ~KineticScroller();
    void attachTo(QWidget* scrollArea);
    void detach(void);
    void startMotion(const QPointF& velocity);
    void stopMotion(void);

signals:
    void scrollBy(const QPoint& d);
    void zoomBy(qreal factor, const QPoint& center);

protected:
    bool eventFilter(QObject* object, QEvent* event);
    void timerEvent(QTimerEvent*);

private:
    static const qreal Friction;
    static const int TimeInterval;
    static const int NumKineticDataSamples;

    struct KineticData {
        KineticData(void) : t(0) { /* ... */ }
        KineticData(const QPoint& p, int t) : p(p), t(t) { /* ... */ }
        QPoint p;
        int t;
    };

    QWidget* mScrollArea;
    bool mDragging;
    QPoint mLastMousePos;
    QVector<KineticData> mKineticData;
    QTime mMouseMoveTimer;
    int mTimer;
    QPointF mVelocity;
    bool mControlPressed;
};

#endif // __KINETICSCROLLER_H_
