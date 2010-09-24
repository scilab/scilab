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

#ifndef __STRING_HH__
#define __STRING_HH__

#include <string>
#include <list>
#include "types.hxx"

using std::string;
namespace types
{
    class String : public GenericType
    {
    public :
                                String(size_t _iRows, size_t _iCols);
                                String(size_t _iRows, size_t _iCols, wchar_t** _pstData);
                                String(const wchar_t *_pstData);
                            	String(const char *_pstData);
        virtual                 ~String();

        GenericType*            get_col_value(size_t _iPos);

        wchar_t**               string_get() const;
        wchar_t*                string_get(size_t _iRows, size_t _iCols) const;
        wchar_t*                string_get(size_t _iPos) const;

        bool                    string_set(wchar_t **_pstData);
        bool                    string_set(size_t _iRows, size_t _iCols, const wchar_t *_pstData);
        bool                    string_set(size_t _iPos, const wchar_t *_pstData);

        void                    whoAmI();

        String*                 getAsString(void);
        wstring                  toString(int _iPrecision, int _iLineLen);

        bool                    resize(size_t _iNewRows, size_t _iNewCols);
        bool                    insert(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static String*          insert_new(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, String* _poSource, bool _bAsVector);
        String*                 extract(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector);
        bool                    append(size_t _iRows, size_t _iCols, String *_poSource);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring         getTypeStr() {return L"string";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring         getShortTypeStr() {return L"c";}
        String*                 clone();

    protected :
        RealType                getType();//			{ return RealString; }

    private :
        wchar_t**               m_pstData;

        void                    all_delete();
        void                    string_delete(size_t _iRows, size_t _iCols);
        void                    string_delete(size_t _iPos);

        void                    CreateString(size_t _iRows, size_t _iCols);
    };
}
#endif /* !__STRING_HH__ */
