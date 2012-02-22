/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef DATA_LOADER_HXX
#define DATA_LOADER_HXX

extern "C"
{

  /**
   * Return the number of data elements for the given object.
   * @param[in] the given object id.
   * @return the number of data elements.
   */
  int getDataSize(char* id);

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
  void fillVertices(char* id, float* BUFF, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask);

  /**
   * Fill the given buffer with color data from the given object.
   * @param[in] the id of the given object.
   * @param[in] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   * @param[in] the number of coordinate taken by one element in the buffer (3 for RGB, 4 for RGBA).
   */
  void fillColors(char* id, float* BUFF, int bufferLength, int elementsSize);

  /**
   * Fill the given buffer with texture coordinates data from the given object.
   * @param[in] the id of the given object.
   * @param[in] the buffer to fill.
   * @param[in] the buffer length in number of elements.
   */
  void fillTextureCoordinates(char* id, float* buffer, int bufferLength);

  /**
   * Return the number of indices for the given object.
   * @param[in] the given object id.
   * @return the object's number of indices.
   */
  int getIndicesSize(char* id);

  /**
   * Fill the given buffer with indices data of the given object.
   * @param[in] the id of the given object.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length.
   * @param[in] the bit mask specifying whether logarithmic coordinates are used.
   * @return the number of indices actually written.
   */
  int fillIndices(char* id, int* BUFF, int bufferLength, int logMask);

  /**
   * Return the number of indices for the wire of the given object.
   * @param[in] the given object id.
   * @return the object's number of indices.
   */
  int getWireIndicesSize(char* id);

  /**
   * Fill the given buffer with wire indices data of the given object.
   * @param[in] the id of the given object.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length.
   * @param[in] the bit mask specifying whether logarithmic coordinates are used.
   * @return the number of indices actually written.
   */
  int fillWireIndices(char* id, int* BUFF, int bufferLength, int logMask);

  /**
   * Return the number of mark indices of the given object.
   * @param[in] the given object id.
   * @return the number of mark indices.
   */
  int getMarkIndicesSize(char* id);

  /**
   * Fill the given buffer with mark indices data of the given object.
   * @param[in] the id of the given object.
   * @param[out] the buffer to fill.
   * @param[in] the buffer length.
   * @return the number of indices actually written.
   */
  int fillMarkIndices(char* id, int* BUFF, int bufferLength);

}

#endif DATA_LOADER_HXX
