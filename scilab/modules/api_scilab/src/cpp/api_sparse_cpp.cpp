/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include "api_sparse.hxx"
#include "double.hxx"

namespace api_scilab
{
Sparse::Sparse(Double& _pdbl)
{
    types::Double* pdbl = (types::Double*)_pdbl.getReturnVariable();
    if (pdbl)
    {
        data = new types::Sparse(*pdbl);
    }
    else
    {
        data = new types::Sparse(1, 1);
        data->set(0, 0);
    }

    created = true;
}

Sparse::Sparse(int _iRows, int _iCols, bool _bComplex)
{
    data = new types::Sparse(_iRows, _iCols, _bComplex);
    created = true;
}

Sparse::Sparse(Double& _dblAadj, Double& _dblAdjncy, Double& _dblSrc, std::size_t _iRows, std::size_t _iCols)
{
    types::Double* pdblAadj = (types::Double*)_dblAadj.getReturnVariable();
    types::Double* pdblAdjncy = (types::Double*)_dblAdjncy.getReturnVariable();
    types::Double* pdblSrc = (types::Double*)_dblSrc.getReturnVariable();

    if (pdblAadj && pdblAdjncy && pdblSrc)
    {
        data = new types::Sparse(*pdblAadj, *pdblAdjncy, *pdblSrc, _iRows, _iCols);
    }
    else
    {
        data = new types::Sparse(1, 1);
        data->set(0, 0);
    }
    created = true;
}

Sparse* Sparse::getAsSparse(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isSparse())
    {
        return new Sparse(*_pIT->getAs<types::Sparse>());
    }

    return NULL;
}

int Sparse::getSize()
{
    return data->getSize();
}

int Sparse::getRows()
{
    if (data)
    {
        return data->getRows();
    }

    return 0;
}

int Sparse::getCols()
{
    if (data)
    {
        return data->getCols();
    }

    return 0;
}

bool Sparse::isScalar()
{
    if (data)
    {
        return data->isScalar();
    }

    return false;
}

bool Sparse::isComplex()
{
    if (data)
    {
        return data->isComplex();
    }

    return false;
}

double Sparse::getReal(int _iRows, int _iCols) const
{
    if (data)
    {
        return data->getReal(_iRows, _iCols);
    }

    return 0;
}

double Sparse::getReal(int _iIndex) const
{
    if (data)
    {
        return data->getReal(_iIndex);
    }

    return 0;
}

double Sparse::get(int _iRows, int _iCols) const
{
    if (data)
    {
        return data->get(_iRows, _iCols);
    }

    return 0;
}

double Sparse::get(int _iIndex) const
{
    if (data)
    {
        return data->get(_iIndex);
    }

    return 0;
}

std::complex<double> Sparse::getImg(int _iRows, int _iCols) const
{
    if (data)
    {
        return data->getImg(_iRows, _iCols);
    }

    return 0;
}

std::complex<double> Sparse::getImg(int _iIndex) const
{
    if (data)
    {
        return data->getImg(_iIndex);
    }

    return 0;
}

void Sparse::setComplex(bool _bComplex)
{
    if (data && data->isComplex() == false)
    {
        data->toComplex();
    }
}

int* Sparse::getNbItemByRow(int* _piNbItemByRows)
{
    if (data)
    {
        return data->getNbItemByRow(_piNbItemByRows);
    }

    return NULL;
}

int* Sparse::getColPos(int* _piColPos)
{
    if (data)
    {
        return data->getColPos(_piColPos);
    }

    return NULL;
}

int Sparse::nonZeros() const
{
    if (data)
    {
        return (int)data->nonZeros();
    }

    return 0;
}

}
