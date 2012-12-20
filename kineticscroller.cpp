// Copyright (c) 2011-2012 Oliver Lau <ola@ct.de>
// All rights reserved.

#include "kineticscroller.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <QtCore/QtDebug>
#include <qmath.h>

const qreal KineticScroller::Friction = 0.85;
const int KineticScroller::TimeInterval = 20;
const int KineticScroller::NumKineticDataSamples = 5;


KineticScroller::KineticScroller(QWidget* scrollArea, QObject* parent)
    : QObject(parent)
    , mScrollArea(scrollArea)
    , mDragging(false)
    , mTimer(0)
    , mControlPressed(false)
{
    if (mScrollArea)
        attachTo(mScrollArea);
}


KineticScroller::~KineticScroller()
{
    stopMotion();
    detach();
}


void KineticScroller::startMotion(const QPointF& velocity)
{
    mVelocity = velocity;
    if (mTimer == 0)
        mTimer = startTimer(TimeInterval);
}


void KineticScroller::stopMotion(void)
{
    if (mTimer) {
        killTimer(mTimer);
        mTimer = 0;
    }
    mVelocity = QPointF(0, 0);
}


void KineticScroller::detach(void)
{
    if (mScrollArea)
        mScrollArea->removeEventFilter(this);
}


void KineticScroller::attachTo(QWidget* scrollArea)
{
    Q_ASSERT_X(scrollArea != NULL, "KineticScroller::attachTo()", "QScrollArea not given");
    detach();
    mScrollArea = scrollArea;
    mScrollArea->installEventFilter(this);
}


bool KineticScroller::eventFilter(QObject*, QEvent* event)
{
    bool doFilterEvent = true;
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        const QMouseEvent* const mouseEvent = reinterpret_cast<const QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            stopMotion();
            mScrollArea->setCursor(Qt::ClosedHandCursor);
            mLastMousePos = mouseEvent->pos();
            mDragging = true;
            mMouseMoveTimer.start();
            mKineticData.clear();
        }
        break;
    }
    case QEvent::MouseMove: {
        const QMouseEvent* const mouseEvent = reinterpret_cast<const QMouseEvent*>(event);
        if (mDragging) {
            emit scrollBy(mLastMousePos - mouseEvent->pos());
            mKineticData.push_back(KineticData(mouseEvent->pos(), mMouseMoveTimer.elapsed()));
            if (mKineticData.count() > NumKineticDataSamples)
                mKineticData.pop_front();
            mLastMousePos = mouseEvent->pos();
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        const QMouseEvent* const mouseEvent = reinterpret_cast<const QMouseEvent*>(event);
        if (mDragging) {
            mDragging = false;
            mScrollArea->setCursor(Qt::OpenHandCursor);
            if (mKineticData.count() == NumKineticDataSamples) {
                const int timeSinceLastMoveEvent = mMouseMoveTimer.elapsed() - mKineticData.last().t;
                if (timeSinceLastMoveEvent < 100) {
                    const QPoint& dp = mKineticData.first().p - mouseEvent->pos();
                    const int dt = mMouseMoveTimer.elapsed() - mKineticData.first().t;
                    startMotion(1000 * dp / dt / TimeInterval);
                }
            }
        }
        break;
    }
    case QEvent::KeyPress: {
        const QKeyEvent* const keyEvent = reinterpret_cast<const QKeyEvent*>(event);
        mControlPressed = (keyEvent->key() == Qt::Key_Control);
        doFilterEvent = false;
        break;
    }
    case QEvent::KeyRelease: {
        const QKeyEvent* const keyEvent = reinterpret_cast<const QKeyEvent*>(event);
        mControlPressed = !(keyEvent->key() == Qt::Key_Control);
        doFilterEvent = false;
        break;
    }
    case QEvent::Wheel: {
        const QWheelEvent* const wheelEvent = reinterpret_cast<const QWheelEvent*>(event);
        emit zoomBy(qPow(1.1 + (mControlPressed? 0.5 : 0), 8e-3 * wheelEvent->delta()), wheelEvent->pos());
        break;
    }
    default:
        doFilterEvent = false;
        break;
    }

    return doFilterEvent;
}


void KineticScroller::timerEvent(QTimerEvent*)
{
    if (mVelocity.manhattanLength() > 1.4) {
        emit scrollBy(mVelocity.toPoint());
        mVelocity *= Friction;
    }
    else stopMotion();
}
