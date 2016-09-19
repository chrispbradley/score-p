/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


#ifndef VERTICALRULERCONTROLLER_H
#define VERTICALRULERCONTROLLER_H

#include "AbstractRulerController.h"

class VerticalRulerController : public AbstractRulerController
{
public:
    VerticalRulerController(QWidget* parent);
    virtual void doPaint(QRect viewport, QPainter& painter);
    virtual int pixelCoordToRulerCoord(int pixelCoord, bool oppositeDirection = false);
    virtual double pixelCoordToRulerCoordF(int pixelCoord, bool oppositeDirection = false);
};


#endif // VERTICALRULERCONTROLLER_H
