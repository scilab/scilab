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

#ifndef NGON_GRID_DATA_H
#define NGON_GRID_DATA_H

#include <string>

#include "NgonData.hxx"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

/**
 * n-gons grid data stub class
 * To be fully implemented
 */

class NgonGridData : public NgonData
{
private :

    /** The x coordinates array */
    double* xCoordinates;

    /** The y coordinates array */
    double* yCoordinates;

    /** The z coordinates array */
    double* zCoordinates;

    /** The grid x size */
    int xSize;

    /** The grid y size */
    int ySize;

public :
    /* To be implemented */
    NgonGridData(void)
    {
        numGons = 0;
        numVerticesPerGon = 0;

        xCoordinates = NULL;
        yCoordinates = NULL;
        zCoordinates = NULL;

        xSize = 0;
        ySize = 0;
    }

    /* To be implemented */
    double* getData()
    {
        return zCoordinates;
    }

    /* To be implemented */
    void setData(double* data)
    {

    }

};

#endif
