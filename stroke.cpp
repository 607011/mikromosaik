// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "stroke.h"

int Stroke::boundaryExcess(void) const
{
    const QRectF& boundary = mPath.boundingRect(); // expensive(!) function
    int result = NoExcess;
    if (boundary.left() < 0)
        result |= ExcessLeft;
    if (boundary.right() > 1)
        result |= ExcessRight;
    if (boundary.top() < 0)
        result |= ExcessTop;
    if (boundary.bottom() > 1)
        result |= ExcessBottom;
    return result;
}
