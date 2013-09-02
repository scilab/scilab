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

#ifndef DATA_3D_HXX
#define DATA_3D_HXX

#include <string>

#include "DataProperties.hxx"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

/**
 * An abstract 3D data class
 * To be completed
 */

class Data3D
{
protected:
    /* To be added: a member indicating how color is specified (per-vertex or per-facet) */

public :
    /**
     * Constructor
     */
    Data3D(void);

    /**
     * Destructor
     */
    virtual ~Data3D(void);

    /**
     * Returns the identifier associated to a property name
     * @param[in] propertyName the property name
     * @return the property identifier
     */
    virtual int getPropertyFromName(int propertyName);

    /**
     * Sets a data property
     * @param[in] property the property identifier
     * @param[in] value a pointer to the property values
     * @param[in] numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    virtual int setDataProperty(int property, void const* value, int numElements);

    /**
     * Returns a data property
     * @param[in] property the property identifier
     * @param[out] a pointer to a pointer to the returned property values
     */
    virtual void getDataProperty(int property, void **_pvData);
};

#endif
