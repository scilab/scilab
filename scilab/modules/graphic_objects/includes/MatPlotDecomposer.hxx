/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Pierre Lando
 *  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef MATPLOT_DECOMPOSER_HXX
#define MATPLOT_DECOMPOSER_HXX

class MatPlotDecomposer
{
public:
    static int getTextureWidth(char* id);
    static int getTextureHeight(char* id);
    static int fillTextureData(char* id, unsigned char* buffer, int bufferLength);
    static int fillTextureData(char* id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height);
    static int getTextureData(char* id, void ** address, unsigned int * size);
    static void disposeTextureData(char * id, unsigned char * buffer);
    static int getTextureImageType(char * id);
    static int getTextureDataType(char * id);
    static int getTextureGLType(char * id);
    static int isTextureRowOrder(char * id);
};

#endif
