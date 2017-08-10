/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
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

#ifndef DATA_LOADER_HXX
#define DATA_LOADER_HXX

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    void * address;
    unsigned int size;
} JavaDirectBuffer;

/**
 * Return the texture width for the given object.
 * @param[in] the given object id.
 * @return the texture width for the given object.
 */
int getTextureWidth(int id);

/**
 * Return the texture height for the given object.
 * @param[in] the given object id.
 * @return the texture height for the given object.
 */
int getTextureHeight(int id);

/**
 * Fill the given buffer with texture data.
 * @param[in] the id of the object texture.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length in number of elements.
 * @return the length of writed data.
 */
int fillTextureData(int id, unsigned char* buffer, int bufferLength);

/**
 * Fill the given buffer with sub-texture data.
 * @param[in] the id of the object texture.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length in number of elements.
 * @param[in] the x-coordinate of the first pixel.
 * @param[in] the y-coordinate of the first pixel.
 * @param[in] the width of the sub-texture.
 * @param[in] the length of the sub-texture.
 * @return the length of writed data.
 */
int fillSubTextureData(int id, unsigned char* buffer, int bufferLength, int x, int y, int width, int height);

/**
 * Return the number of data elements for the given object.
 * @param[in] the given object id.
 * @return the number of data elements.
 */
int getDataSize(int id);

/**
 * Fill the given buffer with vertex data from the given object.
 * @param[in] the id of the given object.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length in number of elements.
 * @param[in] the number of coordinate taken by one element in the buffer.
 * @param[in] this byte mask specifies which coordinates are filled (1 for X, 2 for Y, 4 for Z).
 * @param[in] the conversion scale factor to apply to data.
 * @param[in] the conversion translation factor to apply to data.
 * @param[in] the bit mask specifying whether logarithmic coordinates are used.
 */
void fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double scale[], double translation[], int logMask);

/**
 * Fill the given buffer with normal data from the given object.
 * @param[in] the id of the given object.
 * @param[in] the buffer with position data.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length in number of elements.
 * @param[in] the number of coordinate taken by one element in the buffer.
 */
void fillNormals(int id, float* position, float* buffer, int bufferLength, int elementsSize);


/**
 * Fill the given buffer with color data from the given object.
 * @param[in] the id of the given object.
 * @param[in] the buffer to fill.
 * @param[in] the buffer length in number of elements.
 * @param[in] the number of coordinate taken by one element in the buffer (3 for RGB, 4 for RGBA).
 */
void fillColors(int id, float* buffer, int bufferLength, int elementsSize);

/**
 * Fill the given buffer with texture coordinates data from the given object.
 * @param[in] the id of the given object.
 * @param[in] the buffer to fill.
 * @param[in] the buffer length in number of elements.
 */
void fillTextureCoordinates(int id, float* buffer, int bufferLength);

/**
 * Return the number of indices for the given object.
 * @param[in] the given object id.
 * @return the object's number of indices.
 */
int getIndicesSize(int id);

/**
 * Fill the given buffer with indices data of the given object.
 * @param[in] the id of the given object.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length.
 * @param[in] the bit mask specifying whether logarithmic coordinates are used.
 * @return the number of indices actually written.
 */
int fillIndices(int id, int* buffer, int bufferLength, int logMask);

/**
 * Return the number of indices for the wire of the given object.
 * @param[in] the given object id.
 * @return the object's number of indices.
 */
int getWireIndicesSize(int id);

/**
 * Fill the given buffer with wire indices data of the given object.
 * @param[in] the id of the given object.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length.
 * @param[in] the bit mask specifying whether logarithmic coordinates are used.
 * @return the number of indices actually written.
 */
int fillWireIndices(int id, int* buffer, int bufferLength, int logMask);

/**
 * Return the number of mark indices of the given object.
 * @param[in] the given object id.
 * @return the number of mark indices.
 */
int getMarkIndicesSize(int id);

/**
 * Fill the given buffer with mark indices data of the given object.
 * @param[in] the id of the given object.
 * @param[out] the buffer to fill.
 * @param[in] the buffer length.
 * @return the number of indices actually written.
 */
int fillMarkIndices(int id, int* buffer, int bufferLength);

/**
 * Get texture data of the given object.
 * @param[in] the id of the given object.
 * @return a JavaDirectBuffer struct which will be converted into a java.nio.ByteBuffer.
 */
JavaDirectBuffer getTextureData(int id);

/**
 * Get texture image type of the given object.
 * @param[in] id the id of the given object.
 * @return the image type.
 */
int getTextureImageType(int id);

/**
 * Get the Scilab data type associated with the texture of the given object.
 * @param[in] id the id of the given object.
 * @return the image type (0 for RGB, 1 for RGBA, 2 for GRAY).
 */
int getTextureDataType(int id);

/**
 * Get texture OpenGL type of the given object.
 * @param[in] id the id of the given object.
 * @return the OpenGL type.
 */
int getTextureGLType(int id);

/**
 * Dispose texture data of the given object.
 * @param[in] id the id of the given object.
 * @param[in] buffer the address of the buffer to dipose.
 */
void disposeTextureData(int id, unsigned char * buffer);

/**
 * Dispose texture data of the given object.
 * @param[in] id the id of the given object.
 * @return 1 if the texture is ROW_MAJOR_ORDER
 */
int isTextureRowOrder(int id);

/**
 * Set tha availability of the ABGR extension
 * @param[in] isAvailable 0 if it is not available
 */
void setABGRExt(int isAvailable);

#ifdef __cplusplus
} //extern "C"
#endif

#endif /* DATA_LOADER_HXX */
