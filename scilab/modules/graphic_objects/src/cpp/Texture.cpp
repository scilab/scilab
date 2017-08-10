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

#include "Texture.hxx"
#include <cstring>

float Texture::GrayR[256];
float Texture::GrayG[256];
float Texture::GrayB[256];
unsigned char Texture::RGB8R[256];
unsigned char Texture::RGB8G[256];
unsigned char Texture::RGB8B[256];
unsigned char Texture::R12[256];
const bool Texture::isGrayTableInit = Texture::initGrayTables();
const bool Texture::isRGB8TableInit = Texture::initRGB8Tables();
const bool Texture::isR12TableInit = Texture::initR12Table();

#define _DOUBLE_TO_UCHAR_(x) ((x) < 0 ? 0 : ((x) > 1 ? 0xFF : ((unsigned char)((x) * 0xFF))))

bool Texture::getImage(void const * data, const int numElements, const DataType datatype, const ImageType imagetype, void ** dest, unsigned int * dataSize, GLType * gltype)
{
    unsigned int size = 0;
    bool ret = false;

    switch (imagetype)
    {
        case MATPLOT_NONE:
            break;

        case MATPLOT_RGB :
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                    size = numElements * sizeof(int);
                    break;
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                    size = numElements * sizeof(float) * 3;
                    break;
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                    size = numElements * 3;
                    break;
                case MATPLOT_Int :
                case MATPLOT_UInt :
                    size = numElements * sizeof(int);
                    break;
                case MATPLOT_Short :
                case MATPLOT_UShort :
                    size = numElements * sizeof(short);
                    break;
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                    size = numElements * sizeof(int);
                    break;
            }
            break;
        case MATPLOT_RGBA :
        case MATPLOT_ARGB :
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                    size = numElements * sizeof(int);
                    break;
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                    size = numElements * sizeof(float) * 4;
                    break;
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                    size = numElements * 4;
                    break;
                case MATPLOT_Int :
                case MATPLOT_UInt :
                    size = numElements * sizeof(int);
                    break;
                case MATPLOT_Short :
                case MATPLOT_UShort :
                    size = numElements * sizeof(short);
                    break;
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                    size = numElements * sizeof(int);
                    break;
            }
            break;
        case MATPLOT_GRAY :
        case MATPLOT_RED :
        case MATPLOT_GREEN :
        case MATPLOT_BLUE :
            if (datatype == MATPLOT_Short || datatype == MATPLOT_UShort)
            {
                size = numElements * sizeof(short);
            }
            else if (datatype == MATPLOT_HM3_Double || datatype == MATPLOT_HM4_Double || datatype == MATPLOT_Double)
            {
                size = numElements * sizeof(float);
            }
            else
            {
                size = numElements;
            }
            break;
        case MATPLOT_INDEX :
            switch (datatype)
            {
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                    size = numElements;
                    break;
                case MATPLOT_Int :
                case MATPLOT_UInt :
                    size = numElements * sizeof(int);
                    break;
                case MATPLOT_Short :
                case MATPLOT_UShort :
                    size = numElements * sizeof(short);
                    break;
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                    size = numElements * sizeof(double);
                    break;
                default:
                    break;
            }
            break;
        case MATPLOT_RGB_332 :
            size = numElements;
            break;
        case MATPLOT_RGB_444 :
        case MATPLOT_RGB_555 :
        case MATPLOT_RGBA_4444 :
        case MATPLOT_RGBA_5551 :
            size = numElements * sizeof(short);
            break;
    }

    if (size != *dataSize)
    {
        ret = true;
        *dataSize = size;
        *dest = new unsigned char[size];
    }

    switch (imagetype)
    {
        case MATPLOT_RGB :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = R[i];
                        ucdata[k + 1] = G[i];
                        ucdata[k + 2] = B[i];
                        k += 3;
                    }
                    *gltype = MATPLOT_GL_RGB;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    float * fdata = (float *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[k] = (float)R[i];
                        fdata[k + 1] = (float)G[i];
                        fdata[k + 2] = (float)B[i];
                        k += 3;
                    }

                    *gltype = MATPLOT_GL_RGB_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_Char :
                case MATPLOT_UChar :
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGB;
                    break;
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (ui[i] >> 16) & 0xFF;
                        ucdata[k + 1] = (ui[i] >> 8) & 0xFF;
                        ucdata[k + 2] = ui[i] & 0xFF;
                        k += 3;
                    }

                    *gltype = MATPLOT_GL_RGB;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (ui[i] >> 24) & 0xFF;
                        ucdata[k + 1] = (ui[i] >> 16) & 0xFF;
                        ucdata[k + 2] = (ui[i] >> 8) & 0xFF;
                        k += 3;
                    }

                    *gltype = MATPLOT_GL_RGB;
                    break;
                }
                case MATPLOT_Short :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UShort :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (((unsigned int)d[i]) >> 16) & 0xFF;
                        ucdata[k + 1] = (((unsigned int)d[i]) >> 8) & 0xFF;
                        ucdata[k + 2] = ((unsigned int)d[i]) & 0xFF;
                        k += 3;
                    }
                    *gltype = MATPLOT_GL_RGB;
                    break;
                }
            }
            break;
        }
        case MATPLOT_RGBA :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = R[i];
                        ucdata[k + 1] = G[i];
                        ucdata[k + 2] = B[i];
                        ucdata[k + 3] = 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    const unsigned char * A = B + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = R[i];
                        ucdata[k + 1] = G[i];
                        ucdata[k + 2] = B[i];
                        ucdata[k + 3] = A[i];
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_HM3_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    float * fdata = (float *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[k] = (float)R[i];
                        fdata[k + 1] = (float)G[i];
                        fdata[k + 2] = (float)B[i];
                        fdata[k + 3] = 1.0f;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_FLOAT;
                    break;
                }
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    const double * A = B + numElements;
                    float * fdata = (float *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[k] = (float)R[i];
                        fdata[k + 1] = (float)G[i];
                        fdata[k + 2] = (float)B[i];
                        fdata[k + 3] = (float)A[i];
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (ui[i] >> 16) & 0xFF;
                        ucdata[k + 1] = (ui[i] >> 8) & 0xFF;
                        ucdata[k + 2] = ui[i] & 0xFF;
                        ucdata[k + 3] = 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (ui[i] >> 24) & 0xFF;
                        ucdata[k + 1] = (ui[i] >> 16) & 0xFF;
                        ucdata[k + 2] = (ui[i] >> 8) & 0xFF;
                        ucdata[k + 3] = ui[i] & 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_Short :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UShort :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (((unsigned int)d[i]) >> 24) & 0xFF;
                        ucdata[k + 1] = (((unsigned int)d[i]) >> 16) & 0xFF;
                        ucdata[k + 2] = (((unsigned int)d[i]) >> 8) & 0xFF;
                        ucdata[k + 3] = ((unsigned int)d[i]) & 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                }
            }
            break;
        }
        case MATPLOT_ARGB :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = R[i];
                        ucdata[k + 1] = G[i];
                        ucdata[k + 2] = B[i];
                        ucdata[k + 3] = 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * A = (const unsigned char *)data;
                    const unsigned char * R = A + numElements;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = R[i];
                        ucdata[k + 1] = G[i];
                        ucdata[k + 2] = B[i];
                        ucdata[k + 3] = A[i];
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_HM3_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    float * fdata = (float *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[k] = (float)R[i];
                        fdata[k + 1] = (float)G[i];
                        fdata[k + 2] = (float)B[i];
                        fdata[k + 3] = 1.0f;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_FLOAT;
                    break;
                }
                case MATPLOT_HM4_Double :
                {
                    const double * A = (const double *)data;
                    const double * R = A + numElements;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    float * fdata = (float *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[k] = (float)R[i];
                        fdata[k + 1] = (float)G[i];
                        fdata[k + 2] = (float)B[i];
                        fdata[k + 3] = (float)A[i];
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    const unsigned char * uc = (const unsigned char *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < size; i += 4)
                    {
                        ucdata[i] = uc[i + 1];
                        ucdata[i + 1] = uc[i + 2];
                        ucdata[i + 2] = uc[i + 3];
                        ucdata[i + 3] = uc[i];
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (ui[i] >> 16) & 0xFF;
                        ucdata[k + 1] = (ui[i] >> 8) & 0xFF;
                        ucdata[k + 2] = ui[i] & 0xFF;
                        ucdata[k + 3] = 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (ui[i] >> 16) & 0xFF;
                        ucdata[k + 1] = (ui[i] >> 8) & 0xFF;
                        ucdata[k + 2] = ui[i] & 0xFF;
                        ucdata[k + 3] = (ui[i] >> 24) & 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                    break;
                }
                case MATPLOT_Short :
                {
                    const unsigned short * us = (const unsigned short *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (us[i] >> 12) | (us[i] << 4);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UShort :
                {
                    const unsigned short * us = (const unsigned short *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (us[i] >> 15) | (us[i] << 1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    unsigned int k = 0;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[k] = (((unsigned int)d[i]) >> 16) & 0xFF;
                        ucdata[k + 1] = (((unsigned int)d[i]) >> 8) & 0xFF;
                        ucdata[k + 2] = ((unsigned int)d[i]) & 0xFF;
                        ucdata[k + 3] = (((unsigned int)d[i]) >> 24) & 0xFF;
                        k += 4;
                    }
                    *gltype = MATPLOT_GL_RGBA_BYTE;
                }
            }
            break;
        }
        case MATPLOT_GRAY :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) (GrayR[R[i]] + GrayG[G[i]] + GrayB[B[i]]);
                    }
                    *gltype = MATPLOT_GL_GRAY;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float) (0.299 * R[i] + 0.587 * G[i] + 0.114 * B[i]);
                    }
                    *gltype = MATPLOT_GL_GRAY_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_GRAY;
                    break;
                }
                case MATPLOT_UShort :
                case MATPLOT_Short :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_GRAY_16;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) (GrayR[(ui[i] >> 24) & 0xFF] + GrayG[(ui[i] >> 16) & 0xFF] + GrayB[(ui[i] >> 8) & 0xFF]);
                    }
                    *gltype = MATPLOT_GL_GRAY;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) (GrayR[(ui[i] >> 16) & 0xFF] + GrayG[(ui[i] >> 8) & 0xFF] + GrayB[ui[i] & 0xFF]);
                    }
                    *gltype = MATPLOT_GL_GRAY;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float)d[i];
                    }
                    *gltype = MATPLOT_GL_GRAY_FLOAT;
                    break;
                }
            }
            break;
        }
        case MATPLOT_INDEX :
        {
            memcpy(*dest, data, size);
            *gltype = MATPLOT_GL_RGBA_BYTE;
            break;
        }
        case MATPLOT_RED :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    memcpy(*dest, data, numElements);
                    *gltype = MATPLOT_GL_RED;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float)d[i];
                    }
                    *gltype = MATPLOT_GL_RED_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RED;
                    break;
                }
                case MATPLOT_Short :
                case MATPLOT_UShort :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RED_16;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) ((ui[i] >> 16) & 0xFF);
                    }
                    *gltype = MATPLOT_GL_RED;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) ((ui[i] >> 24) & 0xFF);
                    }
                    *gltype = MATPLOT_GL_RED;
                    break;
                }
            }
            break;
        }
        case MATPLOT_GREEN :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    memcpy(*dest, (unsigned char *)data + numElements, numElements);
                    *gltype = MATPLOT_GL_GREEN;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * d = (const double *)data + numElements;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float)d[i];
                    }
                    *gltype = MATPLOT_GL_GREEN_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_GREEN;
                    break;
                }
                case MATPLOT_Short :
                case MATPLOT_UShort :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_GREEN_16;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) ((ui[i] >> 8) & 0xFF);
                    }
                    *gltype = MATPLOT_GL_GREEN;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) ((ui[i] >> 16) & 0xFF);
                    }
                    *gltype = MATPLOT_GL_GREEN;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float)d[i];
                    }
                    *gltype = MATPLOT_GL_GREEN_FLOAT;
                    break;
                }
            }
            break;
        }
        case MATPLOT_BLUE :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    memcpy(*dest, (unsigned char *)data + 2 * numElements, numElements);
                    *gltype = MATPLOT_GL_BLUE;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * d = (const double *)data + 2 * numElements;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float)d[i];
                    }
                    *gltype = MATPLOT_GL_BLUE_FLOAT;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_BLUE;
                    break;
                }
                case MATPLOT_Short :
                case MATPLOT_UShort :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_BLUE_16;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) (ui[i] & 0xFF);
                    }
                    *gltype = MATPLOT_GL_BLUE;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char) ((ui[i] >> 8) & 0xFF);
                    }
                    *gltype = MATPLOT_GL_BLUE;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    float * fdata = (float *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        fdata[i] = (float)d[i];
                    }
                    *gltype = MATPLOT_GL_BLUE_FLOAT;
                    break;
                }
            }
            break;
        }
        case MATPLOT_RGB_332 :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char)(RGB8R[R[i]] | RGB8G[G[i]] | RGB8B[B[i]]);
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    unsigned char * ucdata = (unsigned char *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char)(RGB8R[_DOUBLE_TO_UCHAR_(R[i])] | RGB8G[_DOUBLE_TO_UCHAR_(G[i])] | RGB8B[_DOUBLE_TO_UCHAR_(B[i])]);
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char)(RGB8R[(ui[i] & 0xFF0000) >> 16] | RGB8G[(ui[i] & 0xFF00) >> 8] | RGB8B[ui[i] & 0xFF]);
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char)(RGB8R[(ui[i] & 0xFF000000) >> 24] | RGB8G[(ui[i] & 0xFF0000) >> 16] | RGB8B[(ui[i] & 0xFF00) >> 8]);
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_Short :
                {
                    const unsigned short * us = (const unsigned short *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char)(((us[i] & 0xE00) >> 4) | ((us[i] & 0xE0) >> 3) | ((us[i] & 0xC) >> 2));
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_UShort :
                {
                    const unsigned short * us = (const unsigned short *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        ucdata[i] = (unsigned char)(((us[i] & 0xE000) >> 8) | ((us[i] & 0xE00) >> 7) | ((us[i] & 0xC0) >> 6));
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned char * ucdata = (unsigned char *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        const unsigned int ui = (unsigned int)d[i];
                        ucdata[i] = (unsigned char)(RGB8R[(ui & 0xFF000000) >> 24] | RGB8G[(ui & 0xFF0000) >> 16] | RGB8B[(ui & 0xFF00) >> 8]);
                    }
                    *gltype = MATPLOT_GL_RGB_332;
                    break;
                }
            }
            break;
        }
        case MATPLOT_RGB_444 :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[R[i]] << 12) | (R12[G[i]] << 8) | (R12[B[i]] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[_DOUBLE_TO_UCHAR_(R[i])] << 12) | (R12[_DOUBLE_TO_UCHAR_(G[i])] << 8) | (R12[_DOUBLE_TO_UCHAR_(B[i])] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[(ui[i] & 0xFF0000) >> 16] << 12) | (R12[(ui[i] & 0xFF00) >> 8] << 8) | (R12[ui[i] & 0xFF] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[(ui[i] & 0xFF000000) >> 24] << 12) | (R12[(ui[i] & 0xFF0000) >> 16] << 8) | (R12[(ui[i] & 0xFF00) >> 8] << 4) | (R12[ui[i] & 0xFF]));
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UShort :
                case MATPLOT_Short :
                {
                    const unsigned short * us = (const unsigned short *)data;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (us[i] << 4) | 0xF;
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        const unsigned int ui = (unsigned int)d[i];
                        usdata[i] = (unsigned short)((R12[(ui & 0xFF0000) >> 16] << 12) | (R12[(ui & 0xFF00) >> 8] << 8) | (R12[ui & 0xFF] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
            }
            break;
        }
        case MATPLOT_RGB_555 :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((R[i] & 0xF8) << 8) | ((G[i] & 0xF8) << 3) | ((B[i] & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM3_Double :
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((_DOUBLE_TO_UCHAR_(R[i]) & 0xF8) << 8) | ((_DOUBLE_TO_UCHAR_(G[i]) & 0xF8) << 3) | ((_DOUBLE_TO_UCHAR_(B[i]) & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((ui[i] & 0xF80000) >> 8) | ((ui[i] & 0xF800) >> 5) | ((ui[i] & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((ui[i] & 0xF8000000) >> 16) | ((ui[i] & 0xF80000) >> 13) | ((ui[i] & 0xF800) >> 10) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_UShort :
                case MATPLOT_Short :
                {
                    const unsigned short * ui = (const unsigned short *)data;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (ui[i] << 1) | 0x1;
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        const unsigned int ui = (unsigned int)d[i];
                        usdata[i] = (unsigned short)(((ui & 0xF80000) >> 13) | ((ui & 0xF800) >> 5) | ((ui & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
            }
            break;
        }
        case MATPLOT_RGBA_4444 :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[R[i]] << 12) | (R12[G[i]] << 8) | (R12[B[i]] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    const unsigned char * A = B + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[R[i]] << 12) | (R12[G[i]] << 8) | (R12[B[i]] << 4) | R12[A[i]]);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM3_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[_DOUBLE_TO_UCHAR_(R[i])] << 12) | (R12[_DOUBLE_TO_UCHAR_(G[i])] << 8) | (R12[_DOUBLE_TO_UCHAR_(B[i])] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    const double * A = B + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[_DOUBLE_TO_UCHAR_(R[i])] << 12) | (R12[_DOUBLE_TO_UCHAR_(G[i])] << 8) | (R12[_DOUBLE_TO_UCHAR_(B[i])] << 4) | R12[_DOUBLE_TO_UCHAR_(A[i])]);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[(ui[i] & 0xFF0000) >> 16] << 12) | (R12[(ui[i] & 0xFF00) >> 8] << 8) | (R12[ui[i] & 0xFF] << 4) | 0xF);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)((R12[(ui[i] & 0xFF000000) >> 24] << 12) | (R12[(ui[i] & 0xFF0000) >> 16] << 8) | (R12[(ui[i] & 0xFF00) >> 8] << 4) | R12[ui[i] & 0xFF]);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_UShort :
                case MATPLOT_Short :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        const unsigned int ui = (unsigned int)d[i];
                        usdata[i] = (unsigned short)((R12[(ui & 0xFF000000) >> 24] << 12) | (R12[(ui & 0xFF0000) >> 16] << 8) | (R12[(ui & 0xFF) >> 8] << 4) | R12[ui & 0xFF]);
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
            }
            break;
        }
        case MATPLOT_RGBA_5551 :
        {
            switch (datatype)
            {
                case MATPLOT_HM3_Char :
                case MATPLOT_HM3_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((R[i] & 0xF8) << 8) | ((G[i] & 0xF8) << 3) | ((B[i] & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM4_Char :
                case MATPLOT_HM4_UChar :
                {
                    const unsigned char * R = (const unsigned char *)data;
                    const unsigned char * G = R + numElements;
                    const unsigned char * B = G + numElements;
                    const unsigned char * A = B + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((R[i] & 0xF8) << 8) | ((G[i] & 0xF8) << 3) | ((B[i] & 0xF8) >> 2) | ((A[i] & 0x1)));
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM3_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((_DOUBLE_TO_UCHAR_(R[i]) & 0xF8) << 8) | ((_DOUBLE_TO_UCHAR_(G[i]) & 0xF8) << 3) | ((_DOUBLE_TO_UCHAR_(B[i]) & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM4_Double :
                {
                    const double * R = (const double *)data;
                    const double * G = R + numElements;
                    const double * B = G + numElements;
                    const double * A = B + numElements;
                    unsigned short * usdata = (unsigned short *) * dest;
                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((_DOUBLE_TO_UCHAR_(R[i]) & 0xF8) << 8) | ((_DOUBLE_TO_UCHAR_(G[i]) & 0xF8) << 3) | ((_DOUBLE_TO_UCHAR_(B[i]) & 0xF8) >> 2) | (_DOUBLE_TO_UCHAR_(A[i]) & 0x1));
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Char :
                case MATPLOT_HM1_UChar :
                case MATPLOT_UChar :
                case MATPLOT_Char :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_Int :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((ui[i] & 0xF80000) >> 13) | ((ui[i] & 0xF800) >> 5) | ((ui[i] & 0xF8) >> 2) | 0x1);
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_UInt :
                {
                    const unsigned int * ui = (const unsigned int *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        usdata[i] = (unsigned short)(((ui[i] & 0xF8000000) >> 21) | ((ui[i] & 0xF80000) >> 13) | ((ui[i] & 0xF800) >> 10) | ((ui[i] & 0xFF) >> 7));
                    }
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_UShort :
                case MATPLOT_Short :
                {
                    memcpy(*dest, data, size);
                    *gltype = MATPLOT_GL_RGBA_5551;
                    break;
                }
                case MATPLOT_HM1_Double :
                case MATPLOT_Double :
                {
                    const double * d = (const double *)data;
                    unsigned short * usdata = (unsigned short *) * dest;

                    for (int i = 0; i < numElements; i++)
                    {
                        const unsigned int ui = (unsigned int)d[i];
                        usdata[i] = (unsigned short)(((ui & 0xF8000000) >> 21) | ((ui & 0xF80000) >> 13) | ((ui & 0xF800) >> 10) | ((ui & 0xFF) >> 7));
                    }
                    *gltype = MATPLOT_GL_RGBA_4444;
                    break;
                }
            }
            break;
        }
    }

    return ret;
}


bool Texture::initGrayTables()
{
    for (int c = 0; c < 256; c++)
    {
        GrayR[c] = 0.299f * (float)c;
        GrayG[c] = 0.587f * (float)c;
        GrayB[c] = 0.114f * (float)c;
    }

    return true;
}

bool Texture::initRGB8Tables()
{
    for (int c = 0; c < 256; c++)
    {
        RGB8R[c] = (c / 36) << 5;
        RGB8G[c] = (c / 36) << 2;
        RGB8B[c] = c / 81;
    }

    return true;
}

bool Texture::initR12Table()
{
    for (int c = 0; c < 256; c++)
    {
        R12[c] = c / 17;
    }

    return true;
}
