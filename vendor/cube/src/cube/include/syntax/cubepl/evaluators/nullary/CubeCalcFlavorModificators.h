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


#ifndef __CALC_FLAVOR_MODIFICATORS_H
#define __CALC_FLAVOR_MODIFICATORS_H 0


#include <float.h>
#include <cmath>

#include "CubeTypes.h"
namespace cube
{
class CalcFlavorModificator
{
protected:
    bool verbose_execution;
public:

    CalcFlavorModificator()
    {
        verbose_execution = false;
    };

    virtual
    ~CalcFlavorModificator()
    {
    };

    virtual
    CalculationFlavour
    flavour( CalculationFlavour cf = CUBE_CALCULATE_SAME )
    {
        return cf;
    };

    virtual void
    print() = 0;

    virtual
    double
    eval( double arg1, double arg2 )
    {
        return 0.;
    };

    inline
    virtual
    void
    set_verbose_execution( bool _v )
    {
        verbose_execution = _v;
    }
};


class CalcFlavorModificatorSame : public CalcFlavorModificator
{
    virtual void
    print()
    {
        std::cout << "*";
    };
};

class CalcFlavorModificatorIncl : public CalcFlavorModificator
{
public:
    virtual
    ~CalcFlavorModificatorIncl()
    {
    };
    CalculationFlavour
    flavour( CalculationFlavour cf = CUBE_CALCULATE_INCLUSIVE )
    {
        cf = CUBE_CALCULATE_INCLUSIVE;
        return cf;
    };
    virtual void
    print()
    {
        std::cout << "i";
    };
};


class CalcFlavorModificatorExcl : public CalcFlavorModificator
{
public:
    virtual
    ~CalcFlavorModificatorExcl()
    {
    };
    CalculationFlavour
    flavour( CalculationFlavour cf = CUBE_CALCULATE_EXCLUSIVE )
    {
        cf = CUBE_CALCULATE_EXCLUSIVE;
        return cf;
    };
    virtual void
    print()
    {
        std::cout << "e";
    };
};
};

#endif
