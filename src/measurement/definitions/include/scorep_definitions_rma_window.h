/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2013,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2013,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2013,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2013,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_INTERNAL_DEFINITIONS_H
#error "Do not include this header directly, use SCOREP_Definitions.h instead."
#endif

#ifndef SCOREP_PRIVATE_DEFINITIONS_RMA_WINDOW_H
#define SCOREP_PRIVATE_DEFINITIONS_RMA_WINDOW_H


/**
 * @file
 *
 *
 */



SCOREP_DEFINE_DEFINITION_TYPE( InterimRmaWindow )
{
    SCOREP_DEFINE_DEFINITION_HEADER( InterimRmaWindow );

    SCOREP_StringHandle              name_handle;
    SCOREP_InterimCommunicatorHandle communicator_handle;
};


SCOREP_DEFINE_DEFINITION_TYPE( RmaWindow )
{
    SCOREP_DEFINE_DEFINITION_HEADER( RmaWindow );

    SCOREP_StringHandle       name_handle;
    SCOREP_CommunicatorHandle communicator_handle;
};


SCOREP_RmaWindowHandle
SCOREP_Definitions_NewRmaWindow( const char*               name,
                                 SCOREP_CommunicatorHandle communicatorHandle );


/**
 * Associate the parameter tuple with a process unique RMA window handle.
 *
 * @param name A meaningful name for the RMA window, e.g. 'MPI window'
 * or 'Gfx Card 1'. The string will be copied.
 *
 * @param communicatorHandle Underlying communicator
 *
 * @return A process unique RMA window handle to be used in calls to other
 * SCOREP_RMA* functions.
 */
SCOREP_InterimRmaWindowHandle
SCOREP_Definitions_NewInterimRmaWindow( const char*                      name,
                                        SCOREP_InterimCommunicatorHandle communicatorHandle );


SCOREP_RmaWindowHandle
SCOREP_Definitions_NewUnifiedRmaWindow( const char*               name,
                                        SCOREP_CommunicatorHandle communicatorHandle );


void
scorep_definitions_unify_rma_window( SCOREP_RmaWindowDef*                 definition,
                                     struct SCOREP_Allocator_PageManager* handlesPageManager );


#endif /* SCOREP_PRIVATE_DEFINITIONS_RMA_WINDOW_H */
