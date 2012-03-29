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
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP String : public ArrayOf<wchar_t*>
    {
    public :
                                String(int _iRows, int _iCols);
                                String(int _iDims, int* _piDims);
                                String(int _iRows, int _iCols, wchar_t** _pstData);
                                String(const wchar_t *_pstData);
                            	String(const char *_pstData);
        virtual                 ~String();

        void                    whoAmI();

        virtual bool            set(int _iPos, wchar_t* _pwstData);
        virtual bool            set(int _iPos, const wchar_t* _pwstData);
        virtual bool            set(int _iRows, int _iCols, const wchar_t* _pwstData);
        virtual bool            set(int _iRows, int _iCols, wchar_t* _pwstData);
        virtual bool            set(wchar_t** _pwstData);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        void                    subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);
        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"string";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"c";}
        InternalType*           clone();

        RealType                getType();//			{ return RealString; }
        bool                    isString() { return true; }

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
    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<wchar_t*>; //String
#endif

#endif /* !__STRING_HXX__ */
