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

#ifndef DATA_3D_H
#define DATA_3D_H

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
    Data3D(void);

    virtual ~Data3D(void);

    virtual int getPropertyFromName(char* propertyName);

    virtual int setDataProperty(int property, void* value, int numElements);

    virtual void* getDataProperty(int property);

    virtual void setData(double* data, int numElements) { return; }

    virtual void setDataX(double* data, int numElements) { return; }

    virtual void setDataY(double* data, int numElements) { return; }

    virtual void setDataZ(double* data, int numElements) { return; }

    virtual double* getData(void) { return NULL; }

    virtual double* getDataX(void) { return NULL; }

    virtual double* getDataY(void) { return NULL; }

    virtual double* getDataZ(void) { return NULL; }

    virtual int setXCoordinatesShift(double* data, int numElements) { return 0; }

    virtual double* getXCoordinatesShift(void) { return NULL; }

    virtual int setYCoordinatesShift(double* data, int numElements) { return 0; }

    virtual double* getYCoordinatesShift(void) { return NULL; }

    virtual int setZCoordinatesShift(double* data, int numElements) { return 0; }

    virtual double* getZCoordinatesShift(void) { return NULL; }

    virtual int getNumElements(void) { return -1; }

    virtual void setNumElements(int numElements) { return; }

    int setNumElementsArray(int* numElementsArray) { return 0; }

    virtual void setNumVerticesPerGon(int numVerticesPerGon) { return; }

    virtual int getNumVerticesPerGon(void) { return 0; }

    virtual void setNumGons(int numGons) { return; }

    virtual int getNumGons(void) { return 0; }

    virtual void setZCoordinatesSet(int zCoordinatesSet) { return; }

    virtual int getZCoordinatesSet(void) { return 0; };

    virtual int getXCoordinatesShiftSet(void) { return 0; }

    virtual void setXCoordinatesShiftSet(int xShiftSet) { return; }

    virtual int getYCoordinatesShiftSet(void) { return 0; }

    virtual void setYCoordinatesShiftSet(int yShiftSet) { return; }

    virtual int getZCoordinatesShiftSet(void) { return 0; }

    virtual void setZCoordinatesShiftSet(int zShiftSet) { return; }
};

#endif
