/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

// This code is separated in matrixpoly.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __POLYNOM_HXX__
#define __POLYNOM_HXX__

#include "arrayof.hxx"
#include "singlepoly.hxx"
#include "double.hxx"

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

namespace types
{
class EXTERN_AST Polynom : public ArrayOf<SinglePoly*>
{
public :
    Polynom();
    Polynom(const std::wstring& _szVarName, int _iRows, int _iCols);
    Polynom(const std::wstring& _szVarName, int _iRows, int _iCols, const int *_piRank);
    Polynom(const std::wstring& _szVarName, int _iDims, const int* _piDims);
    Polynom(const std::wstring& _szVarName, int _iDims, const int* _piDims, const int *_piRank);

    virtual                 ~Polynom();

    // FIXME : Should not return NULL
    Polynom*                clone();

    Polynom*                setCoef(int _iRows, int _iCols, Double *_pdblCoef);
    Polynom*                setCoef(int _iIdx, Double *_pdblCoef);

    virtual Polynom*        setComplex(bool _bComplex);

    inline ScilabType       getType(void)
    {
        return ScilabPolynom;
    }

    inline ScilabId         getId(void)
    {
        return isScalar() ? isComplex() ? IdScalarPolynomComplex
               : IdScalarPolynom
               : isComplex() ? IdPolynomComplex
               : IdPolynom;
    }

    bool isComplex(void);

    bool isDollar();
    inline bool             isPoly()
    {
        return true;
    }

    void                    whoAmI(void);
    std::wstring&           getVariableName();
    void                    setVariableName(const std::wstring&);
    bool                    getSizes(int *_piSizes);
    bool                    getRank(int *_piRank);
    int                     getMaxRank(void);
    Double*                 evaluate(Double* _pdblValue);
    void                    updateRank(void);
    Double*                 getCoef(void);
    Polynom*                setCoef(Double *_pCoef);
    Double*                 extractCoef(int _iRank);
    bool                    insertCoef(int _iRank, Double* _pCoef);
    void                    setZeros();
    Polynom*                insert(typed_list* _pArgs, InternalType* _pSource);

    Polynom*                set(int _iPos, SinglePoly* _pS);
    Polynom*                set(int _iRows, int _iCols, SinglePoly* _pS);
    Polynom*                set(SinglePoly** _pS);

    std::wstring            getRowString(int* _piDims, int _iDims, bool _bComplex);
    std::wstring            getColString(int* _piDims, int _iDims, bool _bComplex);
    std::wstring            getMatrixString(int* _piDims, int _iDims, bool _bComplex);


    bool                    operator==(const InternalType& it);
    bool                    operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"polynomial";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"p";
    }

    bool isTrue()
    {
        return false;
    }

    bool transpose(InternalType *& out);
    bool adjoint(InternalType *& out);

    static Polynom* Dollar();

protected :
    std::wstring            m_szVarName;
    void                    createPoly(const std::wstring& _szVarName, int _iDims, const int* _piDims, const int *_piRank);

private :
    virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    virtual SinglePoly*     getNullValue();
    virtual Polynom*        createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual SinglePoly*     copyValue(SinglePoly* _pData);
    virtual void            deleteAll();
    virtual void            deleteImg();
    virtual SinglePoly**    allocData(int _iSize);
    virtual void            deleteData(SinglePoly* data);
};
}

#ifdef _MSC_VER
template class types::ArrayOf<types::SinglePoly*>; //Polynom
#endif
#endif /* !__POLYNOM_HXX__ */
