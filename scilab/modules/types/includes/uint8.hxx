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

#ifndef __UINT8_HXX__
#define __UINT8_HXX__

#include "arrayof.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP UInt8 : public ArrayOf<unsigned char>
    {
    public :
                                    UInt8(unsigned char _ucVal);
                                    UInt8(int _iRows, int _iCols);
                                    UInt8(int _iDims, int* _piDims);
                                    UInt8(int _iRows, int _iCols, unsigned char** _pucData);
                                    ~UInt8();

        InternalType*               clone();


        /*Config management*/
        void                        whoAmI();

        bool                        isInt() { return true; }
        bool                        isUInt8() { return true; }

        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"uint8";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"i";}

    protected :
        RealType				    getType(void){return InternalType::RealUInt8;}

    private :
        virtual bool                subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims);

        virtual unsigned char       getNullValue();
        virtual UInt8*              createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual unsigned char       copyValue(unsigned char _ucData);
        virtual void                deleteAll();
        virtual void                deleteImg();
        virtual unsigned char*      allocData(int _iSize);
    };
}

#ifdef _MSC_VER
template class TYPES_IMPEXP types::ArrayOf<unsigned char>; //UInt8
#endif

#endif /* !__UINT8_HXX__ */
