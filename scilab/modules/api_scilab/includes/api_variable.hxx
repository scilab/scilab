/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/


#ifndef __API_VARIABLE_HXX__
#define __API_VARIABLE_HXX__

#include "dynlib_api_scilab.h"
#include "internal.hxx"

namespace api_scilab
{
class Variable
{
private :
protected :
    bool created;
public :
    Variable() {};
    virtual ~Variable() {};

    virtual types::InternalType::ScilabType getType() = 0;
    void setReturnVariable()
    {
        created = false;
    }

    virtual void* getReturnVariable() = 0;
    virtual int getSize() = 0;
};

class Matrix : public Variable
{
public :
    Matrix() {};
    virtual ~Matrix() {};

    virtual int getDims() = 0;
    virtual int* getDimsArray() = 0;
    virtual int getRows() = 0;
    virtual int getCols() = 0;
    virtual int getSize() = 0;
    virtual bool isScalar() = 0;
    virtual bool isVector() = 0;
    virtual bool isRowVector() = 0;
    virtual bool isColVector() = 0;
    virtual bool isHypermatrix() = 0;
};

template<class XType> //XType is types::Double for example
class VariableT : public Matrix
{
protected :
    XType* data;

public :
    VariableT(): data(nullptr) {};
    virtual ~VariableT() {};

    virtual void* getReturnVariable()
    {
        return (void*)data;
    }

};

template<class Type, class XType> //Type is double and XType is Double
class MatrixT : public VariableT<XType>
{
public :
    virtual Type* getReal()
    {
        return get();
    }
    virtual Type* get()
    {
        if (this->data)
        {
            return this->data->get();
        }

        return NULL;
    }

    virtual Type* getImg()
    {
        if (this->data)
        {
            return this->data->getImg();
        }

        return NULL;
    }

    virtual Type getReal(int _iIndex)
    {
        return get(_iIndex);
    }
    virtual Type get(int _iIndex)
    {
        if (this->data)
        {
            return this->data->get(_iIndex);
        }

        return 0;
    }


    virtual Type getImg(int _iIndex)
    {
        if (this->data)
        {
            return this->data->getImg(_iIndex);
        }

        return 0;
    }

    virtual Type getReal(int* _piCoordinates)
    {
        return get(_piCoordinates);
    }
    virtual Type get(int* _piCoordinates)
    {
        if (this->data)
        {
            int index = this->data->getIndex(_piCoordinates);
            return this->data->get(index);
        }

        return 0;
    }

    virtual Type getImg(int* _piCoordinates)
    {
        if (this->data)
        {
            int index = this->data->getIndex(_piCoordinates);
            return this->data->getImg(index);
        }

        return 0;
    }

    virtual void setReal(int _iIndex, Type _dbl)
    {
        set(_iIndex, _dbl);
    }
    virtual void set(int _iIndex, Type _dbl)
    {
        if (this->data)
        {
            this->data->set(_iIndex, _dbl);
        }
    }

    virtual void setImg(int _iIndex, Type _dbl)
    {
        if (this->data)
        {
            this->data->setImg(_iIndex, _dbl);
        }
    }

    virtual void setReal(int* _piCoordinates, Type _dbl)
    {
        set(_piCoordinates, _dbl);
    }
    virtual void set(int* _piCoordinates, Type _dbl)
    {
        if (this->data)
        {
            int index = this->data->getIndex(_piCoordinates);
            this->data->set(index, _dbl);
        }
    }

    virtual void setImg(int* _piCoordinates, Type _dbl)
    {
        if (this->data)
        {
            int index = this->data->getIndex(_piCoordinates);
            this->data->setImg(index, _dbl);
        }
    }

    virtual void setReal(Type* _pdblCopy)
    {
        set(_pdblCopy);
    }
    virtual void set(Type* _pdblCopy)
    {
        if (this->data)
        {
            this->data->set(_pdblCopy);
        }
    }

    virtual void setImg(Type* _pdblCopy)
    {
        if (this->data)
        {
            this->data->setImg(_pdblCopy);
        }
    }

    virtual int getDims()
    {
        if (this->data)
        {
            return this->data->getDims();
        }

        return 0;
    }

    virtual int* getDimsArray()
    {
        if (this->data)
        {
            return this->data->getDimsArray();
        }

        return NULL;
    }

    virtual int getRows()
    {
        if (this->data)
        {
            return this->data->getRows();
        }

        return 0;
    }

    virtual int getCols()
    {
        if (this->data)
        {
            return this->data->getCols();
        }

        return 0;
    }

    virtual int getSize()
    {
        if (this->data)
        {
            return this->data->getSize();
        }

        return 0;
    }

    virtual bool isScalar()
    {
        if (this->data)
        {
            return this->data->getSize() == 1;
        }

        return false;
    }

    virtual bool isVector()
    {
        if (this->data)
        {
            return this->data->isVector();
        }

        return false;
    }

    virtual bool isRowVector()
    {
        if (this->data)
        {
            if (this->data->isVector() && this->data->getDimsArray()[0] != 1)
            {
                return true;
            }
        }

        return false;
    }

    virtual bool isColVector()
    {
        if (this->data)
        {
            if (this->data->isVector() && this->data->getDimsArray()[1] != 1)
            {
                return true;
            }
        }

        return false;
    }

    virtual bool isHypermatrix()
    {
        if (this->data)
        {
            return this->data->getDims() > 2;
        }

        return false;
    }

    virtual bool isComplex()
    {
        if (this->data)
        {
            return this->data->isComplex();
        }

        return false;
    }

    virtual void setComplex(bool _bComplex)
    {
        if (this->data)
        {
            this->data->setComplex(_bComplex);
        }
    }

};

template <class XType>
class Container : public VariableT<XType>
{

};
}
#endif /* ! __API_VARIABLE_HXX__ */

