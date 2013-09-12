/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef NGON_DATA_HXX
#define NGON_DATA_HXX

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
    /**
     * Constructor
     */
    NgonData(void);

    /**
     * Destructor
     */
    ~NgonData(void);

    /**
     * Returns the identifier associated to a property name
     * @param[in] propertyName the property name
     * @return the property identifier
     */
    int getPropertyFromName(int propertyName);

    /**
     * Sets a data property
     * @param[in] property the property identifier
     * @param[in] value a pointer to the property values
     * @param[in] numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    int setDataProperty(int property, void const* value, int numElements);

    /**
     * Returns a data property
     * @param[in] property the property identifier
     * @param[out] a pointer to a pointer to the returned property values
     */
    void getDataProperty(int property, void **_pvData);

    /**
     * Sets the number of n-gons
     * @param[in] the number of n-gons
     */
    void setNumGons(int numGons);

    /**
     * Returns the number of n-gons
     * @return the number of n-gons
     */
    int getNumGons(void);

    /**
     * Sets the number of vertices making up a single n-gon
     * @param[in] the number of vertices per n-gon
     */
    void setNumVerticesPerGon(int numVerticesPerGon);

    /**
     * Returns the number of vertices making up a single n-gon
     * @return the number of vertices per n-gon
     */
    int getNumVerticesPerGon(void);
};

#endif
