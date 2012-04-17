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

#ifndef __INT8_HXX__
#define __INT8_HXX__

#include "arrayof.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP Int8 : public ArrayOf<char>
    {
    public :
                                    Int8(char _cVal);
                                    Int8(int _iRows, int _iCols);
                                    Int8(int _iDims, int* _piDims);
                                    Int8(int _iRows, int _iCols, char** _pcData);
                                    ~Int8();

        InternalType*               clone();


        /*Config management*/
        void                        whoAmI();

        bool                        isInt() { return true; }
        bool                        isInt8() { return true; }

        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"int8";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"i";}

    protected :
        RealType				    getType(void){return InternalType::RealInt8;}

    private :
        virtual bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

        virtual char                getNullValue();
        virtual Int8*               createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual char                copyValue(char _cData);
        virtual void                deleteAll();
        virtual void                deleteImg();
        virtual char*               allocData(int _iSize);
    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<char>; //Int8
#endif

#endif /* !__INT8_HXX__ */
