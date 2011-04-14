/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __STRUCT_HXX__
#define __STRUCT_HXX__

#include "arrayof.hxx"
#include "singlestruct.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP Struct : public ArrayOf<SingleStruct*>
    {
    public :
                                    ~Struct();
                                    Struct();
                                    Struct(int _iRows, int _iCols);
                        	        Struct(int _iDims, int* _piDims);

    private :
                                    Struct(Struct* _oCellCopyMe);

    public :

        void                        whoAmI(void) { std::cout << "types::Cell"; };

        RealType                    getType(void) { return RealStruct; }
        bool                        isStruct() { return true; }
        bool                        isEmpty();


        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        InternalType*               clone();

        bool                        set(int _iRows, int _iCols, SingleStruct* _pIT);
        bool                        set(int _iRows, int _iCols, const SingleStruct* _pIT);
        bool                        set(int _iIndex, SingleStruct* _pIT);
        bool                        set(int _iIndex, const SingleStruct* _pIT);
        bool                        set(SingleStruct** _pIT);

        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"struct";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"st";};
        virtual bool                isContainer(void) { return true; }
        void                        subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen);
        String*                     getFieldNames();
        bool                        exists(const std::wstring& _sKey);
        bool                        addField(const std::wstring& _sKey);
        virtual std::wstring        toString(int _iPrecision, int _iLineLen);
        std::vector<InternalType*>  extractFields(std::list<std::wstring> _wstFields);

        virtual void                IncreaseRef();
        virtual void                DecreaseRef();

    private :
        virtual SingleStruct*       getNullValue();
        virtual Struct*             createEmpty(int _iDims, int* _piDims, bool _bComplex = false);
        virtual SingleStruct*       copyValue(SingleStruct* _pData);
        virtual void                deleteAll();
        virtual void                deleteImg();
        virtual SingleStruct**      allocData(int _iSize);
    };
}

#endif /* !__STRUCT_HXX__ */
