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


#ifndef __MAX_EVALUATION_H
#define __MAX_EVALUATION_H 0

#include "CubeUnaryEvaluation.h"
namespace cube
{
class MaxEvaluation : public UnaryEvaluation
{
public:
    MaxEvaluation();
    MaxEvaluation( GeneralEvaluation* _arg ) : UnaryEvaluation( _arg )
    {
    };

    virtual
    ~MaxEvaluation();

    virtual
    double
    eval();

    virtual
    double eval( Cnode *, CalculationFlavour, Sysres *, CalculationFlavour   );

    virtual
    double eval( Cnode *, CalculationFlavour );


    virtual
    double*
    eval_row( Cnode*             _cnode,
              CalculationFlavour _cf );


    virtual
    void
    print()
    {
        std::cout << "max( ";
        arguments[ 0 ]->print();
        std::cout << ", ";
        arguments[ 1 ]->print();
        std::cout << ")";
    };

    virtual
    double
    eval( double arg1,
          double arg2 );
};
};

#endif
