/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef NGON_DATA_H
#define NGON_DATA_H

#include <string>

#include "Data3D.hxx"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

/**
 * Abstract class for n-gon data
 */

class NgonData : public Data3D
{
protected :

    /** The total number of n-gons */
    int numGons;

    /** The number of vertices making up a single n-gon */
    int numVerticesPerGon;

public :
    NgonData(void);

    ~NgonData(void);

    /**
     * Returns the value corresponding to a property name
     */
    int getPropertyFromName(char* propertyName);

    /**
     * Sets the data property
     */
    int setDataProperty(int property, void* value, int numElements);

    /**
     * Returns the data property
     */
    void getDataProperty(int property, void **_pvData);

    /**
     * Sets the number of n-gons
     */
    void setNumGons(int numGons);

    /**
     * Returns the number of n-gons
     */
    int getNumGons(void);

    /**
     * Sets the number of vertices making up a single n-gon
     */
    void setNumVerticesPerGon(int numVerticesPerGon);

    /**
     * Returns the number of vertices making up a single n-gon
     */
    int getNumVerticesPerGon(void);

#if 0
    virtual double* getData();

    virtual void setData(double* data);
#endif

};

#endif
