/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef NGONGRIDMATPLOTDATA_DECOMPOSER_H
#define NGONGRIDMATPLOTDATA_DECOMPOSER_H

#include <string>

/**
 * NgonGridMatplotData decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Matplot object's properties.
 * It only has static member functions since it stores no actual state.
 *
 */

class NgonGridMatplotDataDecomposer : public NgonGridDataDecomposer
{

public :

    /**
     * Fills the given buffer with color data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of components taken by one element in the buffer (3 or 4).
     */
    static void fillColors(char* id, float* buffer, int bufferLength, int elementsSize);

    /**
     * Fills the given buffer with indices data of the given object.
     * @param[in] the given object id.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillIndices(char* id, int* buffer, int bufferLength, int logMask);

};

#endif
