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


/**
 * \file CubeIndexHeader.cpp
 * \brief Provides methods for the class IndexHeader
 */


#ifndef __INDEX_HEADER_CPP
#define __INDEX_HEADER_CPP

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "CubeIndexes.h"
#include "CubeIndexHeader.h"
#include "CubeTrafos.h"
#include "CubeIndexMarker.h"
#include "CubeServices.h"
#include "CubeError.h"

using namespace std;
using namespace cube;


IndexHeader::IndexHeader(
    cnodes_number_t  c_count,
    threads_number_t t_count )
{
    index = NULL;
//     value         = NULL;
//     data_type     = type;
    threads_count = t_count;
    cnodes_count  = c_count;

    trafo = new NOPTrafo();
//     selectValue();
}

IndexHeader::IndexHeader(
    cnodes_number_t  c_count,
    threads_number_t t_count,
    IndexFormat      iformat )
{
    index = NULL;
//     value                  = NULL;
    header.named.version   = 0;
    header.named.endianess = 1;
/*    header.named.process_count = proc_count;
    header.named.thread_count = threads_count;
    header.named.cnode_count = cnodes_count;*/
//     header.named.data_type = type;
    header.named.format = iformat;
    threads_count       = t_count;
    cnodes_count        = c_count;
//     data_type           = type;

    trafo = new NOPTrafo();
//     selectValue();
    selectIndex();
}

IndexHeader::~IndexHeader()
{
    delete trafo;
//     delete value;
    delete index;
}


void
IndexHeader::readHeader( fstream& in )
{
    IndexMarker::checkMarker( in );

/*    char Marker[ CUBE_INDEXFILE_MARKER_SIZE + 1 ];
    memset( Marker, 0, CUBE_INDEXFILE_MARKER_SIZE + 1 );
    in.read( Marker, CUBE_INDEXFILE_MARKER_SIZE );
    if ( strcmp( Marker, CUBE_INDEXFILE_MARKER ) )
    {
        throw WrongMarkerInFileError( "Index file marker at the beginning of header in index file is missing or wrong." );
    }
   //     in.seekg(CUBE_INDEXFILE_MARKER_SIZE);*/
    in.read( header.as_array, sizeof( Header ) );
    if ( trafo != NULL )
    {
        delete trafo;
    }
    if ( header.named.endianess == 1 )
    {
        trafo = new NOPTrafo();
    }
    else
    {
        trafo = new SwapBytesTrafo();
    }
    applyTrafo();
//     selectValue();
    selectIndex();
};

void
IndexHeader::readHeader( fileplace_t name )
{
    fstream in;
    in.open( name.first.c_str(), ios::in | ios::binary );
    in.seekg( name.second.first );
    readHeader( in );
    in.close();
};



void
IndexHeader::writeHeader( fstream& out )
{
    // write first the marker, which is acutually not a part of header
    out.write( CUBE_INDEXFILE_MARKER, CUBE_INDEXFILE_MARKER_SIZE );
    out.write( header.as_array, sizeof( Header ) );
};

void
IndexHeader::writeHeader( FILE* fout )
{
    // write first the marker, which is acutually not a part of header
    fwrite( CUBE_INDEXFILE_MARKER, 1, CUBE_INDEXFILE_MARKER_SIZE, fout );
    fwrite( header.as_array, 1, sizeof( Header ), fout );
};

void
IndexHeader::writeHeader( fileplace_t name )
{
    fstream out;
    services::create_path_for_file( name.first.c_str() );
    out.open( name.first.c_str(), ios::out | ios::binary );
    out.seekp( name.second.first );
    writeHeader( out );
    out.close();
};



void
IndexHeader::applyTrafo()
{
    trafo->trafo( ( char* )( &header.named.version ), sizeof( uint16_t ) );
}

void
IndexHeader::selectIndex()
{
    switch ( header.named.format )
    {
        case CUBE_INDEX_FORMAT_DENSE:
            index = new DenseIndex( cnodes_count, threads_count, trafo );
            break;
        case CUBE_INDEX_FORMAT_SPARSE:
            index = new SparseIndex( cnodes_count, threads_count, trafo );
            break;
        case CUBE_INDEX_FORMAT_NONE:
        default:
            throw RuntimeError( "Unknown index format is saved in header" );
    }
}


void
IndexHeader::printSelf()
{
    cout << "-----DUMP---- " << sizeof( Header ) << " --- " << endl;
    for ( unsigned i = 0; i < sizeof( Header ); ++i )
    {
        cout << hex << ( ( unsigned int* )( &header ) )[ i ]  << " ";
    }
    cout << dec << endl;
    cout << "------------------------" << endl;
    cout << "Endianess: " << header.named.endianess << endl;
    cout << "Version: " << header.named.version << endl;

    cout << "Index Format: ";
    switch ( header.named.format )
    {
        case CUBE_INDEX_FORMAT_DENSE:
            cout << " dense" << endl;
            break;
        case CUBE_INDEX_FORMAT_SPARSE:
            cout << " sparse" << endl;
            break;
        case CUBE_INDEX_FORMAT_NONE:
        default:
            throw  RuntimeError( "Unknown index format is saved in header" );
    }
    cout << "------------------------" << endl;
}

#endif
