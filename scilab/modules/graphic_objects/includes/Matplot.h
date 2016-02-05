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

#ifndef MATPLOT_H
#define MATPLOT_H

#include "dynlib_graphic_objects.h"

typedef enum
{
    MATPLOT_HM1_Char = 0,
    MATPLOT_HM1_UChar = 1,
    MATPLOT_HM1_Double = 2,
    MATPLOT_HM3_Char = 3,
    MATPLOT_HM3_UChar = 4,
    MATPLOT_HM3_Double = 5,
    MATPLOT_HM4_Char = 6,
    MATPLOT_HM4_UChar = 7,
    MATPLOT_HM4_Double = 8,
    MATPLOT_Char = 9,
    MATPLOT_UChar = 10,
    MATPLOT_Int = 11,
    MATPLOT_UInt = 12,
    MATPLOT_Short = 13,
    MATPLOT_UShort = 14,
    MATPLOT_Double = 15
} DataType;

typedef enum
{
    MATPLOT_FORTRAN = 0,
    MATPLOT_C = 1
} DataOrder;

typedef enum
{
    MATPLOT_NONE = -1,
    MATPLOT_RGB = 0,
    MATPLOT_RGBA = 1,
    MATPLOT_GRAY = 2,
    MATPLOT_INDEX = 3,
    MATPLOT_RED = 4,
    MATPLOT_GREEN = 5,
    MATPLOT_BLUE = 6,
    MATPLOT_RGB_332 = 7,
    MATPLOT_RGB_444 = 8,
    MATPLOT_RGB_555 = 9,
    MATPLOT_RGBA_4444 = 10,
    MATPLOT_RGBA_5551 = 11,
    MATPLOT_ARGB = 12,
} ImageType;

typedef enum
{
    MATPLOT_GL_RGB = 0,
    MATPLOT_GL_RGB_RGBA = 1,
    MATPLOT_GL_BGR = 2,
    MATPLOT_GL_GRAY = 3,
    MATPLOT_GL_GRAY_16 = 4,
    MATPLOT_GL_RGBA = 5,
    MATPLOT_GL_RGBA_REV = 6,
    MATPLOT_GL_ABGR = 7,
    MATPLOT_GL_RGB_332 = 8,
    MATPLOT_GL_RED = 9,
    MATPLOT_GL_GREEN = 10,
    MATPLOT_GL_BLUE = 11,
    MATPLOT_GL_INTENSITY = 12,
    MATPLOT_GL_RGBA_4444 = 13,
    MATPLOT_GL_RGBA_5551 = 14,
    MATPLOT_GL_RGB_FLOAT = 15,
    MATPLOT_GL_RGBA_FLOAT = 16,
    MATPLOT_GL_GRAY_FLOAT = 17,
    MATPLOT_GL_RED_16 = 18,
    MATPLOT_GL_GREEN_16 = 19,
    MATPLOT_GL_BLUE_16 = 20,
    MATPLOT_GL_RED_FLOAT = 21,
    MATPLOT_GL_GREEN_FLOAT = 22,
    MATPLOT_GL_BLUE_FLOAT = 23,
    MATPLOT_GL_RGBA_BYTE = 24
} GLType;

/**
 * Build an integer which contains the given info
 * @param[in] datatype the data type
 * @param[in] dataorder the data order
 * @param[in] imagetype the image type
 * @return an int
 */
GRAPHIC_OBJECTS_IMPEXP int buildMatplotType(DataType datatype, DataOrder dataorder, ImageType imagetype);

#endif // MATPLOT_H
