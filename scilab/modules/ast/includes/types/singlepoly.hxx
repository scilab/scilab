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
    virtual void            deleteAll() override;
    virtual void            deleteImg() override;

    SinglePoly*             clone() override;
    SinglePoly*             conjugate();

    bool                    isSinglePoly() override
    {
        return true;
    }

    bool                    hasComplexCoef();
    /*Config management*/
    void                    whoAmI() override;

    virtual double          getNullValue() override;
    virtual SinglePoly*     createEmpty(int _iDims, int* _piDims, bool _bComplex) override;
    virtual double*         allocData(int _iSize) override;
    virtual double          copyValue(double _dblData) override;
    virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    bool                    setZeros();
    int                     getRank();
    double                  getDegree();
    bool                    setRank(int _iRank, bool bSave = false);
    bool                    setCoef(Double *_poPow);
    bool                    setCoef(const double *_pdblCoefR, const double *_pdblCoefI);
    bool                    evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI);
    void                    updateRank(void);

    void                    toStringRealImg(const std::wstring& _szVar, std::list<std::wstring>* _pListCoef, int iLineLen);

    bool                    toString(std::wostringstream& ostr) override;

    bool                    operator==(const InternalType& it) override;
    bool                    operator!=(const InternalType& it) override;

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr() const override
    {
        return L"poly";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr() const override
    {
        return L"p";
    }
protected :
    inline ScilabType       getType(void) override
    {
        return ScilabSinglePolynom;
    }
    inline ScilabId         getId(void) override
    {
        return IdSinglePolynom;
    }

private :
    void                    toStringInternal(double *_pdblVal, double *_pdblImg, const std::wstring& _szVar, std::list<std::wstring>* _pListCoef, int iLineLen);

};

SinglePoly* operator*(const SinglePoly& lhs, const SinglePoly& rhs);

}

#endif /* !__POLY_HH__ */
