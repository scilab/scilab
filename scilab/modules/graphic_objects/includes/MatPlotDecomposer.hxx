/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
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

#ifndef MATPLOT_DECOMPOSER_HXX
#define MATPLOT_DECOMPOSER_HXX

class MatPlotDecomposer
{
public:
    static int getTextureWidth(int id);
    static int getTextureHeight(int id);
    static int fillTextureData(int id, unsigned char* buffer, int bufferLength);
    static int fillTextureData(int id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height);
    static int getTextureData(int id, void ** address, unsigned int * size);
    static void disposeTextureData(int id, unsigned char * buffer);
    static int getTextureImageType(int id);
    static int getTextureDataType(int id);
    static int getTextureGLType(int id);
    static int isTextureRowOrder(int id);
};

#endif
