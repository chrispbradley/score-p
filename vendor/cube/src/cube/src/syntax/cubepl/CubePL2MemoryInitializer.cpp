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


#ifndef __CUBEPL2_MEMORY_INITIALIZER_CPP
#define __CUBEPL2_MEMORY_INITIALIZER_CPP 0

#include <vector>
#include <stack>
#include <iostream>
#include <float.h>
#include <cmath>
#include <map>

using namespace std;

#include "CubePL2MemoryInitializer.h"
#include "Cube.h"
#include "CubeMetric.h"
#include "CubeCnode.h"
#include "CubeRegion.h"
#include "CubeSysres.h"


using namespace cube;



void
CubePL2MemoryInitializer:: memory_init()
{
    cubepl2_memory_manager->init();
}

void
CubePL2MemoryInitializer:: memory_new()
{
    cubepl2_memory_manager->new_page();
}

void
CubePL2MemoryInitializer::memory_cleanup()
{
    cubepl2_memory_manager->throw_page();
}


void
CubePL2MemoryInitializer:: memory_setup( cube::Cube* cube )
{
    const std::vector<std::string>&           mirror_urlv     = cube->get_mirrors();
    const std::vector<cube::Metric*>&         metv            = cube->get_metv();
    const std::vector<cube::Metric*>&         root_metv       = cube->get_root_metv();
    const std::vector<cube::Region*>&         regv            = cube->get_regv();
    const std::vector<cube::Cnode*>&          cnodev          = cube->get_cnodev();
    const std::vector<cube::Cnode*>&          fullcnodev      = cube->get_fullcnodev();
    const std::vector<cube::Cnode*>&          root_cnodev     = cube->get_root_cnodev();
    const std::vector<cube::Location*>&       locationv       = cube->get_locationv();
    const std::vector<cube::LocationGroup*>&  location_groupv = cube->get_location_groupv();
    const std::vector<cube::SystemTreeNode*>& stnv            = cube->get_stnv();
    const std::vector<cube::SystemTreeNode*>& root_stnv       = cube->get_root_stnv();

    const std::string& cubename = cube->get_cubename();

    cubepl2_memory_manager->set_row_size( locationv.size() );



    cubepl2_memory_manager->init();
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_MIRRORS, 0, mirror_urlv.size(), CUBEPL_RESERVED_VARIABLE );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_METRICS, 0, metv.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_ROOT_METRICS, 0, root_metv.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_REGIONS, 0, regv.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_CALLPATHS, 0, cnodev.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_ROOT_CALLPATHS, 0, root_cnodev.size(), CUBEPL_RESERVED_VARIABLE );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_LOCATIONS, 0, locationv.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_LOCATION_GROUPS, 0, location_groupv.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_STNS, 0, stnv.size(), CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_ROOT_STNS, 0, root_stnv.size(), CUBEPL_RESERVED_VARIABLE  );

    cubepl2_memory_manager->put( cubepl2::CUBE_FILENAME, 0, cubename, CUBEPL_RESERVED_VARIABLE  );

    uint32_t num_void_procs    = cube->get_number_void_processes();
    uint32_t num_void_thrds    = cube->get_number_void_threads();
    uint32_t num_nonvoid_procs = location_groupv.size() - num_void_procs;
    uint32_t num_nonvoid_thrds = locationv.size() - num_void_thrds;

    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_VOID_LGS, 0, num_void_procs, CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_VOID_LOCS, 0, num_void_thrds, CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_NONVOID_LGS, 0, num_nonvoid_procs, CUBEPL_RESERVED_VARIABLE  );
    cubepl2_memory_manager->put( cubepl2::CUBE_NUM_NONVOID_LOCS, 0, num_nonvoid_thrds, CUBEPL_RESERVED_VARIABLE  );


    // now setup the metrics insformation
    for ( vector<Metric*>::const_iterator miter = metv.begin(); miter != metv.end(); miter++ )
    {
        Metric* _metric = *miter;
        if ( _metric == NULL )
        {
            continue;
        }
        uint32_t _id = _metric->get_id();
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_UNIQ_NAME, _id,  _metric->get_uniq_name(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_DISP_NAME, _id, _metric->get_disp_name(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_URL, _id, _metric->get_url(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_DESCRIPTION, _id, _metric->get_descr(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_DTYPE, _id, _metric->get_dtype(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_UOM, _id, _metric->get_uom(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_EXPRESSION, _id, _metric->get_expression(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_INIT_EXPRESSION, _id, _metric->get_init_expression(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_NUM_CHILDREN, _id, _metric->num_children(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_METRIC_PARENT_ID, _id, ( _metric->get_parent() != 0 ) ? _metric->get_parent()->get_id() : -1., CUBEPL_RESERVED_VARIABLE  );
    }

/*    // now setup the ghost metrics insformation
    for (vector<Metric*>::iterator miter = ghost_metv.begin(); miter != ghost_metv.end(); miter++)
    {
      Metric * _metric = *miter;
      uint32_t _id = _metric->get_id();

    }
 */
// now setup the regions information
    for ( vector<Region*>::const_iterator riter = regv.begin(); riter != regv.end(); riter++ )
    {
        Region*  _region = *riter;
        uint32_t _id     = _region->get_id();
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_NAME, _id,  _region->get_name(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_MANGLED_NAME, _id,  _region->get_mangled_name(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_PARADIGM, _id,  _region->get_paradigm(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_ROLE, _id,  _region->get_role(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_URL, _id, _region->get_url(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_DESCRIPTION, _id,  _region->get_descr(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_MOD, _id,  _region->get_mod(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_BEGIN_LINE, _id,  _region->get_begn_ln(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_REGION_END_LINE, _id,  _region->get_end_ln(), CUBEPL_RESERVED_VARIABLE  );
    }
    // now setup the callpath information
    for ( vector<Cnode*>::const_iterator citer = fullcnodev.begin(); citer != fullcnodev.end(); citer++ )
    {
        Cnode*   _cnode = *citer;
        uint32_t _id    = _cnode->get_id();
        cubepl2_memory_manager->put( cubepl2::CUBE_CALLPATH_MOD, _id,  _cnode->get_mod(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_CALLPATH_LINE, _id,  _cnode->get_line(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_CALLPATH_NUM_CHILDREN, _id,  _cnode->num_children(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_CALLPATH_PARENT_ID, _id,  ( _cnode->get_parent() != NULL ) ? _cnode->get_parent()->get_id() : -1., CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_CALLPATH_CALLEE_ID, _id,  ( _cnode->get_callee() != NULL ) ? _cnode->get_callee()->get_id() : -1., CUBEPL_RESERVED_VARIABLE  );
    }

    // now setup the system informatoin (machines)
    for ( vector<SystemTreeNode*>::const_iterator stniter = stnv.begin(); stniter != stnv.end(); stniter++ )
    {
        SystemTreeNode* _stn = *stniter;
        uint32_t        _id  = _stn->get_id();

        cubepl2_memory_manager->put( cubepl2::CUBE_STN_NAME, _id, _stn->get_name(), CUBEPL_RESERVED_VARIABLE   );
//         cubepl2_memory_manager->put( cubepl2::CUBE_STN_DESCRIPTION, _id, _stn->get_description(), CUBEPL_RESERVED_VARIABLE   );
        cubepl2_memory_manager->put( cubepl2::CUBE_STN_CLASS, _id, _stn->get_class(), CUBEPL_RESERVED_VARIABLE   );
        cubepl2_memory_manager->put( cubepl2::CUBE_STN_PARENT_ID, _id, ( _stn->get_parent() != NULL ) ? _stn->get_parent()->get_id() : -1., CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_STN_PARENT_SYS_ID, _id, ( _stn->get_parent() != NULL ) ? _stn->get_parent()->get_sys_id() : -1., CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_STN_NUM_CHILDREN, _id,  _stn->num_children(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_STN_NUM_LOCATION_GROUPS, _id,  _stn->num_groups(), CUBEPL_RESERVED_VARIABLE  );
    }


    // now setup the system informatoin (location groups)
    for ( vector<LocationGroup*>::const_iterator lgiter = location_groupv.begin(); lgiter != location_groupv.end(); lgiter++ )
    {
        LocationGroup* _lg     = *lgiter;
        uint32_t       _id     = _lg->get_id();
        bool           is_void = false;
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_NAME, _id, _lg->get_name(), CUBEPL_RESERVED_VARIABLE   );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_RANK,   _id, _lg->get_rank(), CUBEPL_RESERVED_VARIABLE );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_TYPE,   _id, _lg->get_type(), CUBEPL_RESERVED_VARIABLE );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_PARENT_ID, _id,  _lg->get_parent()->get_id(), CUBEPL_RESERVED_VARIABLE  );
        is_void = ( _lg->get_name().find( "VOID" ) != string::npos );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_VOID, _id, ( is_void ) ? 1 : 0, CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_PARENT_ID, _id, _lg->get_parent()->get_id(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_PARENT_SYS_ID, _id, _lg->get_parent()->get_sys_id(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_GROUP_NUM_LOCATIONS, _id,  _lg->num_children(), CUBEPL_RESERVED_VARIABLE  );
    }
    // now setup the system informatoin (locations)
    for ( vector<Location*>::const_iterator liter = locationv.begin(); liter != locationv.end(); liter++ )
    {
        Location* _loc    = *liter;
        uint32_t  _id     = _loc->get_id();
        bool      is_void = false;
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_NAME, _id, _loc->get_name(), CUBEPL_RESERVED_VARIABLE   );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_RANK,   _id, _loc->get_rank(), CUBEPL_RESERVED_VARIABLE );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_TYPE,   _id, _loc->get_type(), CUBEPL_RESERVED_VARIABLE );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_PARENT_ID, _id,  _loc->get_parent()->get_id(), CUBEPL_RESERVED_VARIABLE  );
        is_void = ( _loc->get_name().find( "VOID" ) != string::npos );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_VOID, _id, ( is_void ) ? 1 : 0, CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_PARENT_ID, _id, _loc->get_parent()->get_id(), CUBEPL_RESERVED_VARIABLE  );
        cubepl2_memory_manager->put( cubepl2::CUBE_LOCATION_PARENT_SYS_ID, _id, _loc->get_parent()->get_sys_id(), CUBEPL_RESERVED_VARIABLE  );
    }
}

void
CubePL2MemoryInitializer::memory_setup( cube::Metric* metric )
{
    cubepl2_memory_manager->put( cubepl2::CALCULATION_METRIC_ID, 0, metric->get_id(), CUBEPL_VARIABLE  );
}


void
CubePL2MemoryInitializer:: memory_setup( cube::Cnode* cnode )
{
    cubepl2_memory_manager->put( cubepl2::CALCULATION_CALLPATH_ID, 0,  cnode->get_id(), CUBEPL_VARIABLE  );
}


void
CubePL2MemoryInitializer:: memory_setup( cube::Region* region )
{
    cubepl2_memory_manager->put( cubepl2::CALCULATION_REGION_ID, 0,  region->get_id(), CUBEPL_VARIABLE  );
}

void
CubePL2MemoryInitializer::memory_setup(  cube::Sysres* sysres )
{
    cubepl2_memory_manager->put( cubepl2::CALCULATION_SYSRES_ID,   0, sysres->get_id(), CUBEPL_VARIABLE   );
    cubepl2_memory_manager->put( cubepl2::CALCULATION_SYSRES_KIND,   0, sysres->get_kind(), CUBEPL_VARIABLE  );
}

#endif