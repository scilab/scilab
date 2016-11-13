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