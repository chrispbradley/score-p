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



#ifndef __CUBE_TAU2CUBE_CALLS_H
#define __CUBE_TAU2CUBE_CALLS_H



map<const TauMetric*, const Metric*> tmet2cmet;     // /< Connects metrics in TAU und CUBE
map<const TauRegion*, const Region*> treg2creg;     // /< Connects regione in TAU und CUBE
map<const TauCpath*, const Cnode*>   tcpath2ccnode; // /< Connects callpaths in TAU und CUBE
map<const TauLoc*, const Thread*>    tloc2cthrd;    // /< Connects threads in TAU und CUBE

// call tree structure
map<const TauCpath*, const TauCpath*>          tcpath2parent; // /< Creates a tree structure of TAU callpath (object->parent)
map<const TauCpath*, vector<const TauCpath*> > tcpath2childv; // /< Creates a tree structure of TAU callpath (object->child)

const Metric* met_calls;




/**
 * Looks in TAU profile for such root, which contains "cpath" and returns it.
 *
 * It runs over second level of cpath[i] and checks, if "cpath" is saved and identical.
   If not -> breaks and returns NULL. If yes und size of cpath[i] the same -> returns cpath[i];

 */
const TauCpath*
get_parent_cpath( const TauProfile* tauprof, const TauCpath* cpath )
{
    const TauCpath* result = NULL;

    // is root?
    if ( cpath->size() == 1 )
    {
        return NULL;
    }
    for ( int i = 0; i < tauprof->get_ncpaths(); i++ )
    {
        if ( tauprof->get_cpath( i )->size() + 1 !=  cpath->size() )
        {
            continue;
        }

        for ( size_t j = 0; j < cpath->size(); j++ )
        {
            if ( j == cpath->size() - 1 )
            {
                result = tauprof->get_cpath( i );
            }
            else if ( *( *cpath )[ j ] != *( *tauprof->get_cpath( i ) )[ j ] )
            {
                break;
            }
        }
    }
    if ( result == NULL )
    {
        cerr << endl;
        cerr << "ERROR: Incomplete TAU call tree." << endl;
        cerr << "Only 1-level, 2-level or full call-path profiles supported." << endl;
        cerr << *cpath << endl;
        exit( EXIT_FAILURE );
    }
    return result;
}









/**
 * Creates metrics in cube with the same name, character (time or not) and so on
   and saves it in the "connection TAU metric -> CUBE metric"
 */
void
create_metric_dim( const TauProfile* tauprof, Cube* cube )
{
    for ( int i = 0; i < tauprof->get_nmets(); i++ )
    {
        string uom, descr;
        if ( tauprof->get_met( i )->is_time() )
        {
            descr = "Time in seconds";
            uom   = "sec";
        }
        else
        {
            descr = "";
            uom   = "occ";
        }
        tmet2cmet[ tauprof->get_met( i ) ] = cube->def_met( tauprof->get_met( i )->get_name(), tauprof->get_met( i )->get_name(), "INTEGER", uom, "", "", descr, NULL, CUBE_METRIC_INCLUSIVE );
    }
    met_calls = cube->def_met( "CALLS", "CALLS", "INTEGER", "occ", "", "", "Number of calls", NULL, CUBE_METRIC_INCLUSIVE );
}








/**
 * Recursiv creating of cnode's in cube, corresponding to the TAU call path.
 */
void
create_call_tree( const TauCpath* cpath, Cube* cube )
{
    const TauRegion* callee = ( *cpath )[ cpath->size() - 1 ];
    tcpath2ccnode[ cpath ] = cube->def_cnode( const_cast<Region*> ( treg2creg[ callee ] ), "", -1, const_cast<Cnode*> ( tcpath2ccnode[ tcpath2parent[ cpath ] ] ) );
    cerr << "Call-tree node created" << endl;

    for ( size_t i = 0; i < tcpath2childv[ cpath ].size(); i++ )
    {
        create_call_tree( tcpath2childv[ cpath ][ i ], cube );
    }
}








/**
 * Creates the dimension "programm" in cube
 */
void
create_program_dim( const TauProfile* tauprof, Cube* cube )
{
    // identify parent and child nodes
    for ( int i = 0; i < tauprof->get_ncpaths(); i++ )
    {
        cerr << "\n Number of call paths : " << tauprof->get_ncpaths() << endl;
        const TauCpath* child = tauprof->get_cpath( i );
        cerr << "\n Child" << *child << endl;
        const TauCpath* parent = get_parent_cpath( tauprof, child );
        tcpath2parent[ child ] = parent;
        tcpath2childv[ parent ].push_back( child ); // tcpath2childv[NULL] = root vector
    }
    cerr << "\n Path to Parents : " << tcpath2parent.size() << endl;
    cerr << "\n Path to Child : " << tcpath2childv.size() << endl;

    // create pseudo module
    // const Module* pseudo_mod = cube->def_module("UNKNOWN");

    // create regions
    for ( int i = 0; i < tauprof->get_nregions(); i++ )
    {
        treg2creg[ tauprof->get_region( i ) ] = cube->def_region( tauprof->get_region( i )->get_name(), tauprof->get_region( i )->get_name(), "tau", "unknown", -1, -1, "", "", "" );
    }

    // create call tree
    tcpath2ccnode[ NULL ] = NULL;

    // determine whether the profile is a true call-path profile
    bool is_flat = true;
    for ( size_t i = 0; i < tcpath2childv[ NULL ].size(); i++ )
    {
        if ( !tcpath2childv[ tcpath2childv[ NULL ][ i ] ].empty() )
        {
            is_flat = false;
        }
    }

    vector<const TauCpath*> rootv;

    // if true call-path profile then eliminate all roots without children
    for ( size_t i = 0; i < tcpath2childv[ NULL ].size(); i++ )
    {
        if ( is_flat )
        {
            rootv.push_back( tcpath2childv[ NULL ][ i ] );
        }
        else if ( !tcpath2childv[ tcpath2childv[ NULL ][ i ] ].empty() )
        {
            rootv.push_back( tcpath2childv[ NULL ][ i ] );
        }
    }
    cerr << "\n Number of roots : " << rootv.size() << endl;
    for ( size_t i = 0; i < rootv.size(); i++ )
    {
        create_call_tree( rootv[ i ], cube );
    }
}







/**
 * Create a fictional one-node machine, corresponding to the TAU location.
 */
void
create_system_dim( const TauProfile* tauprof, Cube* cube )
{
    // create machine with one node
    const Node* node = cube->def_node( "Node", cube->def_mach( "Machine", "Created by tau2cube" ) );

    // create processes & threads
    map<int, const Process*> proc_id2cproc;

    for ( int i = 0; i < tauprof->get_nlocs(); i++ )
    {
        const TauLoc* loc = tauprof->get_loc( i );

        long proc_id = loc->get_proc_id();
        long thrd_id = loc->get_thrd_id();


        // create process if needed
        const Process*                           cproc = NULL;
        map<int, const Process*>::const_iterator pit   = proc_id2cproc.find( proc_id );
        if ( pit != proc_id2cproc.end() )
        {
            cproc = pit->second;
        }
        else
        {
            cproc                    = cube->def_proc( "", proc_id, const_cast<Node*>( node ) );
            proc_id2cproc[ proc_id ] = cproc;
        }
        // create thread if needed
        tloc2cthrd[ loc ] = cube->def_thrd( "", thrd_id, const_cast<Process*>( cproc ) );
    }
}





/**
 * Fill the severieties matrix in the cube with values in TAU profile.
 */
void
enter_severity( const TauProfile* tauprof, Cube* cube )
{
    for ( int i = 0; i < tauprof->get_nmets(); i++ )
    {
        for ( int j = 0; j < tauprof->get_ncpaths(); j++ )
        {
            for ( int k = 0; k < tauprof->get_nlocs(); k++ )
            {
                const TauMetric* met   = tauprof->get_met( i );
                const TauCpath*  cpath = tauprof->get_cpath( j );
                const TauLoc*    loc   = tauprof->get_loc( k );
                const TauData*   data  = tauprof->get_data( met, cpath, loc );

                // check whether call path has been removed
                if ( tcpath2ccnode.find( cpath ) == tcpath2ccnode.end() )
                {
                    continue;
                }

                double value;
                long   ncalls;


/*   old code for CUBE3 ---> exclusive values

      if ( data == NULL )
        value = 0.0;
      else if ( ! tcpath2childv[cpath].empty() )
        // take exclusive value if call path has children
        value = data->get_excl();
      else
        // take inclusive value if call path is a leaf
        value = data->get_incl();
 */

// / --------- new code , for CUBE 4 --------------
                value = data->get_incl();


                // convert time values from microseconds to seconds
                if ( met->is_time() )
                {
                    value = value / 1000000;
                }

                cube->set_sev( const_cast<Metric*>( tmet2cmet[ met ] ),
                               const_cast<Cnode*>( tcpath2ccnode[ cpath ] ),
                               const_cast<Thread*>( tloc2cthrd[ loc ] ),
                               value );
                cout << endl << "value time :: " << value << endl;
                if ( data == NULL )
                {
                    ncalls = 0;
                }
                else
                {
                    ncalls = data->get_ncalls();
                }

                // take number of calls from first metric
                if ( i == 0 )
                {
                    cube->set_sev( const_cast<Metric*>( met_calls ),
                                   const_cast<Cnode*>( tcpath2ccnode[ cpath ] ),
                                   const_cast<Thread*>( tloc2cthrd[ loc ] ),
                                   ncalls );
                }
                cout << endl << "value ncalls :: " << ncalls << endl;
            }
        }
    }
}



#endif
