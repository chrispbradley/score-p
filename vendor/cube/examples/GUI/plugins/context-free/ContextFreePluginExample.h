/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


#ifndef ContextFreePluginExample_H
#define ContextFreePluginExample_H
#include "ContextFreePlugin.h"
#include "ContextFreeServices.h"

class ContextFreePluginExample : public QObject, public ContextFreePlugin
{
    Q_OBJECT
    Q_INTERFACES( ContextFreePlugin )
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA( IID "ContextFreePluginExample" )
#endif


public:
    // ContextFreePlugin interface
    virtual QString
    name() const;

    virtual void
    opened( ContextFreeServices* service );

    virtual void
    closed();

    virtual void
    version( int& major,
             int& minor,
             int& bugfix ) const;

    virtual QString
    getHelpText() const;

private slots:
    void startAction();
private:
    ContextFreeServices* service;
};

#endif // ContextFreePluginExample_H
