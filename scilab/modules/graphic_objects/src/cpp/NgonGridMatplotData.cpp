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

#include "NgonGridMatplotData.hxx"
#include "Texture.hxx"

#include <climits>

extern "C" {
#include <string.h>
#include <stdlib.h>

#include "graphicObjectProperties.h"
}

const bool NgonGridMatplotData::isLittleEndian = NgonGridMatplotData::initEndian();
bool NgonGridMatplotData::isABGRSupported = false;

NgonGridMatplotData::NgonGridMatplotData(void)
{
    this->data = NULL;
    this->dataSize = 0;
    this->scilabData = NULL;
    this->type = 0;
    this->imagetype = MATPLOT_NONE;
}

NgonGridMatplotData::~NgonGridMatplotData(void)
{
    disposeTextureData();
    if (this->scilabData)
    {
        delete[] (unsigned char *)this->scilabData;
        this->scilabData = NULL;
    }
}

int NgonGridMatplotData::getPropertyFromName(int propertyName)
{
    switch (propertyName)
    {
        case __GO_DATA_MODEL_GRID_SIZE__ :
            return GRID_SIZE;
        case __GO_DATA_MODEL_MATPLOT_BOUNDS__ :
            return MATPLOT_BOUNDS;
        case __GO_DATA_MODEL_MATPLOT_TYPE__ :
            return MATPLOT_TYPE;
        case __GO_DATA_MODEL_Z__ :
            return Z_COORDINATES;
        case __GO_DATA_MODEL_MATPLOT_GL_TYPE__ :
            return MATPLOT_GL_TYPE;
        case __GO_DATA_MODEL_MATPLOT_DATA_INFOS__ :
            return MATPLOT_DATA_INFOS;
        case __GO_DATA_MODEL_MATPLOT_DATA_TYPE__ :
            return MATPLOT_DATA_TYPE;
        case __GO_DATA_MODEL_MATPLOT_DATA_ORDER__ :
            return MATPLOT_DATA_ORDER;
        case __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__ :
            return MATPLOT_IMAGE_TYPE;
        case __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__ :
            return MATPLOT_IMAGE_DATA;
        case __GO_DATA_MODEL_MATPLOT_IMAGE_DATASIZE__ :
            return MATPLOT_IMAGE_DATASIZE;
        default :
            return NgonGridData::getPropertyFromName(propertyName);
    }
}

int NgonGridMatplotData::setDataProperty(int property, void const* value, int numElements)
{
    switch (property)
    {
        case GRID_SIZE :
        {
            return setGridSize((int const*) value);
        }
        case MATPLOT_BOUNDS :
        {
            setBounds((double const*) value);
            break;
        }
        case Z_COORDINATES :
        {
            setDataZ((double const*) value, numElements);
            break;
        }
        case MATPLOT_IMAGE_TYPE :
        {
            return setImageType(*((int const*) value));
        }
        case MATPLOT_DATA_INFOS :
        {
            setDataInfos(*((int const*) value));
            break;
        }
        case MATPLOT_DATA_TYPE :
        {
            setDataType(*((int const*) value));
            break;
        }
        case MATPLOT_DATA_ORDER :
        {
            setDataOrder(*((int const*) value));
            break;
        }
        case MATPLOT_IMAGE_DATA :
        {
            setImageData((void const *) value, numElements);
            break;
        }
        default :
        {
            return NgonGridData::setDataProperty(property, value, numElements);
        }
    }

    return 1;
}

void NgonGridMatplotData::getDataProperty(int property, void **_pvData)
{
    switch (property)
    {
        case MATPLOT_BOUNDS :
        {
            *_pvData = getBounds();
            break;
        }
        case MATPLOT_TYPE :
        {
            ((int *) *_pvData)[0] = getType();
            break;
        }
        case Z_COORDINATES :
        {
            *_pvData = getScilabData();
            break;
        }
        case MATPLOT_IMAGE_TYPE :
        {
            ((int *) *_pvData)[0] = getImageType();
            break;
        }
        case MATPLOT_GL_TYPE :
        {
            ((int *) *_pvData)[0] = getGLType();
            break;
        }
        case MATPLOT_DATA_INFOS :
        {
            ((int *) *_pvData)[0] = getDataInfos();
            break;
        }
        case MATPLOT_DATA_TYPE :
        {
            ((int *) *_pvData)[0] = getDataType();
            break;
        }
        case MATPLOT_DATA_ORDER :
        {
            ((int *) *_pvData)[0] = getDataOrder();
            break;
        }
        case MATPLOT_IMAGE_DATA :
        {
            *_pvData = getImageData();
            break;
        }
        case MATPLOT_IMAGE_DATASIZE :
        {
            ((unsigned int *) *_pvData)[0] = getImageDataSize();
            break;
        }
        default :
        {
            NgonGridData::getDataProperty(property, _pvData);
            break;
        }
    }
}

/*
 * To be done: refactoring with NgonGridData, as these two classes'
 * setGridSize methods are almost identical.
 */
int NgonGridMatplotData::setGridSize(int const* gridSize)
{
    int newXSize = 0;
    int newYSize = 0;
    int xModified = 0;
    int yModified = 0;
    int zModified = 0;
    int result = 0;
    int formerGridSize = 0;

    double* newXCoordinates = NULL;
    double* newYCoordinates = NULL;
    double* newZCoordinates = NULL;

    result = 1;

    xModified = 0;
    yModified = 0;
    zModified = 0;

    if ((gridSize[0] != 1) && (gridSize[1] != 1))
    {
        return 0;
    }

    if ((gridSize[2] != 1) && (gridSize[3] != 1))
    {
        return 0;
    }

    newXSize = gridSize[0] * gridSize[1];
    newYSize = gridSize[2] * gridSize[3];

    if (newXSize != xSize)
    {
        xModified = 1;

        try
        {
            newXCoordinates = new double[newXSize];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }
    }

    if (newYSize != ySize)
    {
        yModified = 1;

        try
        {
            newYCoordinates = new double[newYSize];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }
    }

    if (!result || (!xModified && !yModified))
    {
        return result;
    }

    if (xSize > 0 && ySize > 0)
    {
        formerGridSize = (xSize - 1) * (ySize - 1);
    }
    else
    {
        formerGridSize = 0;
    }

    if ((newXSize - 1) * (newYSize - 1) != formerGridSize)
    {
        zModified = 1;

        try
        {
            newZCoordinates = new double[(newXSize - 1) * (newYSize - 1)];
        }
        catch (const std::exception& e)
        {
            e.what();
            result = 0;
        }
    }

    if (result)
    {
        if (xModified)
        {
            if (xSize > 0)
            {
                delete [] xCoordinates;
            }

            xCoordinates = newXCoordinates;
            xSize = newXSize;
        }

        xDimensions[0] = gridSize[0];
        xDimensions[1] = gridSize[1];

        if (yModified)
        {
            if (ySize > 0)
            {
                delete [] yCoordinates;
            }

            yCoordinates = newYCoordinates;
            ySize = newYSize;
        }

        yDimensions[0] = gridSize[2];
        yDimensions[1] = gridSize[3];

        if (zModified)
        {
            if (formerGridSize > 0)
            {
                delete [] zCoordinates;
            }

            zCoordinates = newZCoordinates;

            numGons = (xSize - 1) * (ySize - 1);

        }

        if (xModified || yModified)
        {
            computeCoordinates();
        }

    }
    else
    {
        /* Failed allocation(s) */

        if (xModified && (newXCoordinates != NULL))
        {
            delete [] newXCoordinates;
        }

        if (yModified && (newYCoordinates != NULL))
        {
            delete [] newYCoordinates;
        }

        if (zModified && (newZCoordinates != NULL))
        {
            delete [] newZCoordinates;
        }

    }

    return result;
}

void NgonGridMatplotData::setBounds(double const* bounds)
{
    if (bounds == NULL)
    {
        /* Type 0 object: bounds are computed from element indices by the setGridSize method */
        type = 0;
    }
    else
    {
        /* To be done: test whether max > min */
        boundingRectangle[0] = bounds[0];
        boundingRectangle[1] = bounds[2];
        boundingRectangle[2] = bounds[1];
        boundingRectangle[3] = bounds[3];

        type = 1;
    }
}

double* NgonGridMatplotData::getBounds(void)
{
    return boundingRectangle;
}

void NgonGridMatplotData::computeCoordinates(void)
{
    if (type == 0)
    {
        for (int i = 0; i < xSize; i++)
        {
            xCoordinates[i] = 0.5 + (double) i;
        }

        for (int i = 0; i < ySize; i++)
        {
            yCoordinates[i] = 0.5 + (double) i;
        }

        boundingRectangle[0] = 0.5;
        boundingRectangle[1] = 0.5 + (double) (xSize - 1);
        boundingRectangle[2] = 0.5;
        boundingRectangle[3] = 0.5 + (double) (ySize - 1);
    }
    else
    {
        double min = boundingRectangle[0];
        double max = boundingRectangle[1];
        int numElements = 0;


        if (xSize == 1)
        {
            numElements = 1;
        }
        else
        {
            numElements = xSize - 1;
        }

        for (int i = 0; i < xSize; i++)
        {
            xCoordinates[i] = min + (double) i * (max - min) / (double) numElements;
        }

        min = boundingRectangle[2];
        max = boundingRectangle[3];

        if (ySize == 1)
        {
            numElements = 1;
        }
        else
        {
            numElements = ySize - 1;
        }

        for (int i = 0; i < ySize; i++)
        {
            yCoordinates[i] = min + (double) i * (max - min) / (double) numElements;
        }
    }
}

void NgonGridMatplotData::setDataZ(double const* data, int numElements)
{
    if (numElements > (xSize - 1) * (ySize - 1))
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        zCoordinates[i] = data[i];
    }
}

int NgonGridMatplotData::setImageType(int imagetype)
{
    const ImageType type = (ImageType)imagetype;

    if (this->imagetype != type)
    {
        int grid[4] = {xSize, 1, ySize, 1};
        if (this->datatype == MATPLOT_UChar || this->datatype == MATPLOT_Char)
        {
            if (this->imagetype == MATPLOT_RGB)
            {
                grid[2] = (ySize - 1) * 3 + 1;
            }
            else if (this->imagetype == MATPLOT_RGBA || this->imagetype == MATPLOT_ARGB)
            {
                grid[2] = (ySize - 1) * 4 + 1;
            }

            if (type == MATPLOT_RGB)
            {
                if ((grid[2] - 1) % 3 != 0)
                {
                    return 0;
                }
                grid[2] = (grid[2] - 1) / 3 + 1;
            }
            else if (type == MATPLOT_RGBA || type == MATPLOT_ARGB)
            {
                if ((grid[2] - 1) % 4 != 0)
                {
                    return 0;
                }
                grid[2] = (grid[2] - 1) / 4 + 1;
            }

            this->setGridSize(grid);
        }

        this->imagetype = type;

        if (this->scilabData)
        {
            setImageData(this->scilabData, (xSize - 1) * (ySize - 1));
        }
    }

    return 1;
}

int NgonGridMatplotData::getImageType()
{
    return (int)this->imagetype;
}

int NgonGridMatplotData::getGLType()
{
    return (int)this->gltype;
}

int NgonGridMatplotData::getDataType()
{
    return (int)this->datatype;
}

void NgonGridMatplotData::setDataType(int datatype)
{
    if (this->datatype != (DataType)datatype)
    {
        this->datatype = (DataType)datatype;
        disposeTextureData();
    }
}

int NgonGridMatplotData::getDataOrder()
{
    return (int)this->dataorder;
}

void NgonGridMatplotData::setDataOrder(int dataorder)
{
    this->dataorder = (DataOrder)dataorder;
}

void * NgonGridMatplotData::getImageData()
{
    if (this->data)
    {
        return this->data;
    }
    else if (this->scilabData)
    {
        setImageData(this->scilabData, (xSize - 1) * (ySize - 1));
        return this->data;
    }

    return NULL;
}

void * NgonGridMatplotData::getScilabData()
{
    return this->scilabData;
}

unsigned int NgonGridMatplotData::getImageDataSize()
{
    return this->dataSize;
}

void NgonGridMatplotData::setDataInfos(int infos)
{
    setDataType(infos & 0xFF);
    setDataOrder((infos & 0xFF00) >> 8);
    setImageType((infos & 0xFF0000) >> 16);
}

int NgonGridMatplotData::getDataInfos()
{
    return buildMatplotType(this->datatype, this->dataorder, this->imagetype);
}

void NgonGridMatplotData::setImageData(void const* data, const int numElements)
{
    if (!data)
    {
        disposeTextureData();
        return;
    }

    unsigned int dataSize = 0;
    const int N = ySize - 1;
    const int M = xSize - 1;
    const int NM = N * M;

    if (numElements > NM)
    {
        return;
    }

    if (data != this->scilabData)
    {
        if (scilabData)
        {
            delete[] (unsigned char *)scilabData;
            scilabData = NULL;
        }

        unsigned int _size;

        switch (datatype)
        {
            case MATPLOT_HM3_Char :
            case MATPLOT_HM3_UChar :
                _size = numElements * 3;
                break;
            case MATPLOT_HM3_Double :
                _size = numElements * sizeof(double) * 3;
                break;
            case MATPLOT_HM4_Char :
            case MATPLOT_HM4_UChar :
                _size = numElements * 4;
                break;
            case MATPLOT_HM4_Double :
                _size = numElements * sizeof(double) * 4;
                break;
            case MATPLOT_HM1_Char :
            case MATPLOT_HM1_UChar :
            case MATPLOT_Char :
            case MATPLOT_UChar :
                _size = numElements;
                break;
            case MATPLOT_Int :
            case MATPLOT_UInt :
                _size = numElements * sizeof(int);
                break;
            case MATPLOT_Short :
            case MATPLOT_UShort :
                _size = numElements * sizeof(short);
                break;
            case MATPLOT_HM1_Double :
            case MATPLOT_Double :
                _size = numElements * sizeof(double);
                break;
        }
        this->scilabData = new unsigned char[_size];
        // todo: on peut ameliorer ca
        // pr certains type de donnees (et certains modes) scilabData == data
        // dc on peut eviter cette copie
        memcpy(this->scilabData, data, _size);
    }

    void * dest = this->data;
    if (this->imagetype == MATPLOT_INDEX)
    {
        this->gltype = MATPLOT_GL_RGBA_BYTE;
    }
    else if (Texture::getImage(data, numElements, this->datatype, this->imagetype, &(this->data), &(this->dataSize), &(this->gltype)))
    {
        if (dest)
        {
            delete[] (unsigned char *)dest;
        }
    }

    //std::cout << this->imagetype << "::" << this->datatype << "::" << this->dataorder << "::" << this->dataSize << "::" << (void*)this->data << std::endl;
}

int NgonGridMatplotData::getType(void)
{
    return type;
}

bool NgonGridMatplotData::initEndian()
{
    const int num = 1;

    return *(char *)&num == 1;
}

void NgonGridMatplotData::setABGRSupported(bool _isABGRSupported)
{
    isABGRSupported = _isABGRSupported;
}

void NgonGridMatplotData::disposeTextureData(void)
{
    if (this->data)
    {
        delete[] (unsigned char *)this->data;
        this->data = NULL;
        this->dataSize = 0;
    }
}
