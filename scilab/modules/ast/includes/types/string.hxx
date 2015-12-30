/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
class EXTERN_AST String : public ArrayOf<char*>
{
public :
    String(int _iRows, int _iCols);
    String(int _iDims, const int* _piDims);
    String(int _iRows, int _iCols, char const* const* _pstData);
    String(const char *_pstData);
    virtual                 ~String();

    void                    whoAmI();

    virtual String*         set(int _iPos, const char* _pcData);
    virtual String*         set(int _iRows, int _iCols, const char* _pcData);
    virtual String*         set(const char* const* _pstrData);

    bool                    operator==(const InternalType& it);
    bool                    operator!=(const InternalType& it);

    bool                    subMatrixToString(std::ostringstream& ostr, int* _piDims, int _iDims) override;
    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string     getTypeStr()
    {
        return "string";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string    getShortTypeStr()
    {
        return "c";
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
    virtual char*           copyValue(char* _pwstData);
    virtual char*           copyValue(const char* _pwstData);
    virtual String*         createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
    virtual char*           getNullValue();
    virtual void            deleteAll();
    virtual void            deleteImg();
    virtual char**          allocData(int _iSize);
    void                    deleteData(char* data);
};
}

#ifdef _MSC_VER
template class types::ArrayOf<char*>; //String
#endif
#endif /* !__STRING_HXX__ */
