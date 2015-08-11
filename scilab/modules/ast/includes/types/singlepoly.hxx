/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __POLY_HH__
#define __POLY_HH__

#include <list>
#include "types.hxx"
#include "double.hxx"

namespace types
{
class EXTERN_AST SinglePoly : public ArrayOf<double>
{
public :
    SinglePoly();
    SinglePoly(double** _pdblCoefR, int _iRank);
    SinglePoly(double** _pdblCoefR, double** _pdblcoefI, int _iRank);

    virtual                 ~SinglePoly();
    virtual void            deleteAll();
    virtual void            deleteImg();

    // FIXME : Should not return NULL;
    SinglePoly*             clone();
    SinglePoly*             conjugate();

    bool                    isSinglePoly()
    {
        return true;
    }
    /*Config management*/
    void                    whoAmI();

    virtual double          getNullValue();
    virtual SinglePoly*     createEmpty(int _iDims, int* _piDims, bool _bComplex);
    virtual double*         allocData(int _iSize);
    virtual double          copyValue(double _dblData);
    virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

    bool                    setZeros();
    int                     getRank();
    bool                    setRank(int _iRank, bool bSave = false);
    bool                    setCoef(Double *_poPow);
    bool                    setCoef(const double *_pdblCoefR, const double *_pdblCoefI);
    bool                    evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI);
    void                    updateRank(void);

    void                    toStringReal(std::wstring _szVar, std::list<std::wstring>* _pListExp , std::list<std::wstring>* _pListCoef);
    void                    toStringImg(std::wstring _szVar, std::list<std::wstring>* _pListExp , std::list<std::wstring>* _pListCoef);

    bool                    toString(std::wostringstream& ostr);

    bool                    operator==(const InternalType& it);
    bool                    operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"poly";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"p";
    }
protected :
    inline ScilabType       getType(void)
    {
        return ScilabSinglePolynom;
    }
    inline ScilabId         getId(void)
    {
        return IdSinglePolynom;
    }

private :
    void                    toStringInternal(double *_pdblVal, std::wstring _szVar, std::list<std::wstring>* _pListExp , std::list<std::wstring>* _pListCoef);

};

SinglePoly* operator*(const SinglePoly& lhs, const SinglePoly& rhs);

}

#endif /* !__POLY_HH__ */
