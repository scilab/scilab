/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
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

#ifndef NGON_GRID_MATPLOT_DATA_HXX
#define NGON_GRID_MATPLOT_DATA_HXX

#include <string>

#include "NgonGridData.hxx"

extern "C" {
#include "BOOL.h"
#include "Matplot.h"
}

/**
 * N-gons grid Matplot data class
 * The xDimensions and yDimensions properties inherited from NgonGridData have no
 * useful purpose, as the x and y coordinates arrays are not currently visible to
 * the user.
 * To be done: refactoring
 */

class NgonGridMatplotData : public NgonGridData
{

private :

    /** The 2D bounding rectangle {xmin, xmax, ymin, ymax} */
    double boundingRectangle[4];

    /**
     * Matplot type
     * Specifies whether x and y coordinates are computed directly from element indices (0)
     * or computed using the 2D bounding rectangle (1)
     */
    int type;
    ImageType imagetype;
    DataType datatype;
    GLType gltype;
    DataOrder dataorder;
    void * data;
    void * scilabData;
    unsigned int dataSize;

    static const bool isLittleEndian;
    static bool isABGRSupported;

protected :

    /**
     * Computes the x and y coordinates depending on the type member
     * If type is equal to 0, x and y coordinates are directly computed from matrix indices
     * and the rectangle bounds are updated
     * else, if equal to 1, the coordinates are computed by interpolating between the bounding
     * rectangle's min and max vertices
     */
    void computeCoordinates(void);

public :

    /**
     * Constructor
     */
    NgonGridMatplotData(void);

    /**
     * Destructor
     */
    ~NgonGridMatplotData(void);

    /**
     * Returns the identifier associated to a property name
     * @param[in] propertyName the property name
     * @return the property identifier
     */
    int getPropertyFromName(int propertyName);

    /**
     * Sets a data property
     * @param[in] property the property identifier
     * @param[in] value a pointer to the property values
     * @param[in] numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    int setDataProperty(int property, void const* value, int numElements);

    /**
     * Returns a data property
     * @param[in] property the property identifier
     * @param[out] a pointer to a pointer to the returned property values
     */
    void getDataProperty(int property, void **_pvData);

    /**
     * Sets the grid's x and y vectors dimensions
     * Resizes the x, y, and z data coordinates arrays if required
     * and must therefore be called prior to any setData call
     * Similar to NgonGridData's setGridSize method, the main difference being how the z data array's size
     * is computed.
     * To be done: refactoring
     * @param[in] gridSize a pointer to a 4-element array: x vector (nb rows, nb cols) and y vector (nb rows, nb cols) dimensions
     * @return 1 if the property has been successfully set, 0 otherwise (failed allocation)
     */
    int setGridSize(int const* gridSize);

    /**
     * Sets the 2D bounding rectangle's coordinates
     * Also sets the internal property type according to the value of its argument
     * To be done: use the same coordinate ordering as the internal array
     * @param[in] a pointer to a 4-element array: {xmin, ymin, xmax, ymax} bounds. If bounds is NULL, type is set to 0, else, it is set to 1
     */
    void setBounds(double const* bounds);

    /**
     * Returns the 2D bounding rectangle's coordinates
     * @return a pointer to the bounding rectangle coordinates array {xmin, xmax, ymin, ymax}
     */
    double* getBounds(void);

    /**
     * Returns the Matplot type
     * @return the Matplot type value
     */
    int getType(void);

    /**
     * Set the info about data
     * @param[in] data info built with buildMatplotType
     */
    void setDataInfos(int datainfos);

    /**
     * Get the info about data
     * @return the info
     */
    int getDataInfos();

    /**
     * Get the OpenGL type
     * @return the type
     */
    int getGLType();

    /**
     * Get the image type
     * @return the type
     */
    int getImageType();

    /**
     * Set the image type
     * @param[in] imagetype the image type
     */
    int setImageType(int imagetype);

    /**
     * Get the data order (MATPLOT_FORTRAN or MATPLOT_C corresponding to COLUMN_MAJOR_ORDER & ROW_MAJOR_ORDER)
     * @return the order
     */
    int getDataOrder();

    /**
     * Set the data order (MATPLOT_FORTRAN or MATPLOT_C corresponding to COLUMN_MAJOR_ORDER & ROW_MAJOR_ORDER)
     * @param[in] the order
     */
    void setDataOrder(int dataOrder);

    /**
     * Set the Scilab data type
     * @param[in] datatype the Scilab data type
     */
    void setDataType(int datatype);

    /**
     * Get the Scilab data type
     * @return the Scilab data type
     */
    int getDataType();

    /**
     * Set the image data
     * @param[in] data the image data
     * @param[in] numElements the number of elements
     */
    void setImageData(void const* data, const int numElements);

    /**
     * Get the image data
     * @return the image data
     */
    void * getImageData();

    /**
     * Get the Scilab data
     * @return the Scilab data
     */
    void * getScilabData();

    /**
     * Get the image data size
     * @return the iamge data size
     */
    unsigned int getImageDataSize();

    /**
     * Dispose the texture data
     */
    void disposeTextureData();

    /**
     * Sets the grid's z data
     * This method is almost identical to NgonGridData's own setDataZ method
     * the only difference being how the maximum number of elements is computed
     * To be done: refactoring
     * @param[in] data a pointer to the z data (numElements values)
     * @param[in] numElements the number of elements to set
     */
    void setDataZ(double const* data, int numElements);

    /**
     * Set true if ABGR extension is supported
     * @param[in] _isABGRSupported true if ABGR extension is supported
     */
    static void setABGRSupported(bool _isABGRSupported);

private:

    /**
     * Init a boolean to true or false according to the endianess
     */
    static bool initEndian();

};

#endif
