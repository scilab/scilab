/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

// This code is separated in string.hxx
// but will be inlined in arrayof.hxx
//
// If you need additionnal headers, please add it in arrayof.hxx

//#ifndef __ARRAYOF_HXX__
//    #error This file must only be include by arrayof.hxx
//#endif

#ifndef __STRING_HXX__
#define __STRING_HXX__

#include <string>
#include <list>

#include "arrayof.hxx"
#include "bool.hxx"
#include "types_transposition.hxx"

namespace types
{
class EXTERN_AST String : public ArrayOf<wchar_t*>
{
public :
    String(int _iRows, int _iCols);
    String(int _iDims, const int* _piDims);
    String(int _iRows, int _iCols, wchar_t const* const* _pstData);
    String(const wchar_t *_pstData);
    String(const char *_pstData);
    virtual                 ~String();

    void                    whoAmI();

    virtual String*         set(int _iPos, const wchar_t* _pwstData);
    virtual String*         set(int _iRows, int _iCols, const wchar_t* _pwstData);
    virtual String*         set(const wchar_t* const* _pwstData);

    virtual String*         set(int _iPos, const char* _pcData);
    virtual String*         set(int _iRows, int _iCols, const char* _pcData);
    virtual String*         set(const char* const* _pstrData);

    bool                    operator==(const InternalType& it);
    bool                    operator!=(const InternalType& it);

    bool                    subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims) override;
    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"string";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"c";
    }
    String*                 clone();

    inline ScilabType       getType()
    {
        return ScilabString;
    }
    inline ScilabId         getId()
    {
        return isScalar() ? IdScalarString : IdString;
    }
    bool                    isString()
    {
        return true;
    }

    bool isTrue()
    {
        return false;
    }

    virtual bool neg(InternalType *& /*out*/)
    {
        return false;
    }

    virtual bool transpose(InternalType *& out);

    virtual ast::Exp*       getExp(const Location& loc);

private :
    void                    deleteString(int _iRows, int _iCols);
    void                    deleteString(int _iPos);

    void                    createString(int _iDims, int* _piDims);
    virtual wchar_t*        copyValue(wchar_t* _pwstData);
    virtual wchar_t*        copyValue(const wchar_t* _pwstData);
    virtual String*         createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual wchar_t*        getNullValue();
    virtual void            deleteAll();
    virtual void            deleteImg();
    virtual wchar_t**       allocData(int _iSize);
    void                    deleteData(wchar_t* data);
};
}

#ifdef _MSC_VER
template class types::ArrayOf<wchar_t*>; //String
#endif
#endif /* !__STRING_HXX__ */
