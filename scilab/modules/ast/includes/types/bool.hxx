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

// This code is separated in bool.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __BOOL_HXX__
#define __BOOL_HXX__

#include "arrayof.hxx"

namespace types
{
class EXTERN_AST Bool : public ArrayOf<int>
{
public :
    static Bool* True()
    {
        return new Bool(1);
    }

    static Bool* False()
    {
        return new Bool(0);
    }

public:
    Bool(int _bReal);
    Bool(int _iRows, int _iCols);
    Bool(int _iDims, const int* _piDims);
    Bool(int _iRows, int _iCols, int **_piData);
    Bool(int _iRows, int _iCols, int * _piData);
    ~Bool();

    InternalType*           clone();


    /*zero or one set filler*/
    bool                    setFalse();
    bool                    setTrue();

    /*Config management*/
    void                    whoAmI();

    bool                    isBool()
    {
        return true;
    }

    bool                    operator==(const InternalType& it);
    bool                    operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring     getTypeStr()
    {
        return L"boolean";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring     getShortTypeStr()
    {
        return L"b";
    }

    virtual bool transpose(InternalType *& out);

protected :
    inline ScilabType       getType(void)
    {
        return ScilabBool;
    }
    inline ScilabId         getId(void)
    {
        return isScalar() ? IdScalarBool : IdBool;
    }

    virtual ast::Exp*       getExp(const Location& loc);

private :
    virtual bool            subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;

    virtual int             getNullValue();
    virtual Bool*           createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual int             copyValue(int _iData);
    virtual void            deleteAll();
    virtual void            deleteImg();
    virtual int*            allocData(int _iSize);
};

}

#ifdef _MSC_VER
template class types::ArrayOf<int>;//Bool
#endif
#endif /* !__BOOL_HXX__ */
