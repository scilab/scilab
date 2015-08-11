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

#ifdef _MSC_VER
#pragma warning( disable : 4275 )
#endif

#ifndef __API_SPARSE_HXX__
#define __API_SPARSE_HXX__

#include "sparse.hxx"
#include "api_double.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP Sparse : public Variable
{
private :
    types::Sparse* data;
    //internal constructor for existing variable
    Sparse(types::Sparse& _sp)
    {
        data = &_sp;
        created = false;
    }

public :
    Sparse(Double& _pdbl);
    Sparse(int _iRows, int _iCols, bool _bComplex = false);
    Sparse(Double& _dblAadj, Double& _dblAdjncy, Double& _dblSrc, std::size_t _iRows, std::size_t _iCols);
    virtual ~Sparse()
    {
        if (created)
        {
            delete data;
        }
    };
    void* getReturnVariable()
    {
        return (void*)data;
    }
    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabSparse;
    }
    static Sparse* getAsSparse(types::InternalType* _pIT);

    int getSize();
    int getRows();
    int getCols();

    bool isScalar();
    void setComplex(bool _bComplex);
    bool isComplex();

    double getReal(int _iRows, int _iCols) const;
    double getReal(int _iIndex) const;
    double get(int _iRows, int _iCols) const;
    double get(int _iIndex) const;
    std::complex<double> getImg(int _iRows, int _iCols) const;
    std::complex<double> getImg(int _iIndex) const;

    int* getNbItemByRow(int* _piNbItemByRows);
    int* getColPos(int* _piColPos);
    int nonZeros() const;

};
}

#endif /* !__API_SPARSE_HXX__ */