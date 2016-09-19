/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2012,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2012,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2012,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2012,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2012,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2012,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2012,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_FILTER_INIT_H
#define SCOREP_FILTER_INIT_H

/**
 * @file
 *
 *
 * Implements the filter initialization interface for the filtering system.
 */

/**
   Initializes the filtering system and parses the configuration file.
 */
void
SCOREP_Filter_Initialize( void );

/**
   Registers the config variables for filtering.
 */
SCOREP_ErrorCode
SCOREP_Filter_Register( void );

/**
   Finalizes the filtering system and frees all memory.
 */
void
SCOREP_Filter_Finalize( void );

#endif /* SCOREP_FILTER_INIT_H */
