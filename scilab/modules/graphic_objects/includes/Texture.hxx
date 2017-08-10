/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#ifndef TEXTURE_HXX
#define TEXTURE_HXX

extern "C" {
#include "Matplot.h"
}

/**
 *
 */

class Texture
{

private :

    static float GrayR[256];
    static float GrayG[256];
    static float GrayB[256];
    static unsigned char RGB8R[256];
    static unsigned char RGB8G[256];
    static unsigned char RGB8B[256];
    static unsigned char R12[256];
    static const bool isGrayTableInit;
    static const bool isRGB8TableInit;
    static const bool isR12TableInit;

public :

    /**
     * Get a texture from data
     * @param[in] data Scilab data to convert to an OpenGL texture
     * @param[in] numElements the number of elements
     * @param[in] datatype the type of the data
     * @param[in] the expected image type
     * @param[out] dest the destination pointer. If its size must be modified, then *dest is allocated.
     * @param[out] the texture size
     * @param[out] gltype the OpenGL data type
     * @return true if *dest has been allocated.
     */
    static bool getImage(void const * data, const int numElements, const DataType datatype, const ImageType imagetype, void ** dest, unsigned int * datatSize, GLType * gltype);

private:
    static bool initGrayTables();
    static bool initRGB8Tables();
    static bool initR12Table();
};

#endif // TEXTURE_HXX
