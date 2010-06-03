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
                                String(int _iRows, int _iCols);
                                String(const char *_pcData);
        virtual                 ~String();

        GenericType*            get_col_value(int _iPos);

        char**                  string_get() const;
        char*                   string_get(int _iRows, int _iCols) const;
        char*                   string_get(int _iPos) const;

        bool                    string_set(char **_pcData);
        bool                    string_set(int _iRows, int _iCols, const char *_pcData);
        bool                    string_set(int _iPos, const char *_pcData);

        void                    whoAmI();

        String*                 getAsString(void);
        string                  toString(int _iPrecision, int _iLineLen);

        bool                    resize(int _iNewRows, int _iNewCols);
        bool                    insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static String*          insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, String* _poSource, bool _bAsVector);
        String*                 extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);
        bool                    append(int _iRows, int _iCols, String *_poSource);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::string     getTypeStr() {return string("string");}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::string     getShortTypeStr() {return string("c");}
        String*                 clone();

    protected :
        RealType                getType();//			{ return RealString; }

    private :
        char**                  m_pstData;

        void                    all_delete();
        void                    string_delete(int _iRows, int _iCols);
        void                    string_delete(int _iPos);

        void                    CreateString(int _iRows, int _iCols);
    };
}
#endif /* !__STRING_HH__ */
