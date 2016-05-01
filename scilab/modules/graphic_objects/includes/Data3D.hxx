/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
