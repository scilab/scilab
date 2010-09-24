
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

#ifndef __INT_HH__
#define __INT_HH__

#include "types.hxx"
#include "core_math.h"

namespace types
{
    class Int : public GenericType
    {
    public :
        enum IntType {
            Type8 = 1,
            TypeUnsigned8 = 11,
            Type16 = 2,
            TypeUnsigned16 = 12,
            Type32 = 4,
            TypeUnsigned32 = 14,
            Type64 = 8,
            TypeUnsigned64 = 18
        };

        /* constructor & destructor */
    protected :
                                Int() {};
        virtual                 ~Int() {};

    public :
        static Int*             createInt(size_t _iRows, size_t _iCols, IntType _iIntType);
        static Int*             insert_new(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, Int* _poSource, bool _bAsVector);

        Int*                    getAsInt(void) { return this; }
        virtual IntType         getIntType() = 0;
        RealType                getType(void);

        virtual bool            data_set(Int* _pData) = 0;
        virtual bool            data_set(size_t _iRows, size_t _iCols, long long _llData) = 0;
        virtual bool            data_set(size_t _iPos, long long _llData) = 0;
        virtual long long       data_get(size_t _iPos) = 0;
        virtual long long       data_get(size_t _iRows, size_t _iCols) = 0;
        virtual void*           data_get() = 0;
        virtual bool            resize(size_t _iNewRows, size_t _iNewCols) = 0;
        virtual GenericType*    get_col_value(size_t _iPos) = 0;
        virtual Int*            extract(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector) = 0;
        virtual bool            insert(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        bool                    append(size_t _iRows, size_t _iCols, Int *_poSource);


        virtual wstring         toString(int _iPrecision, int _iLineLen) = 0;
        virtual Int*            clone() = 0;

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring         getTypeStr() {return L"int";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring         getShortTypeStr() {return L"i";}
    protected :
        /* Only used by Int8,16, ... */
        bool                    extract_size_get(size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector, size_t* _piRows, size_t* _piCols);

    protected :
        IntType                 m_iIntType;
    };
}

#endif /* !__INT_HH__ */
