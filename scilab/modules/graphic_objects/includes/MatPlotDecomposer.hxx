/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
