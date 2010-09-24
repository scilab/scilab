/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LIST_HH__
#define __LIST_HH__

#include <list>
#include "container.hxx"

namespace types
{
    class List : public Container
    {
    public :
                                        List();
                                        ~List();

    private :
                                        List(List *_oListCopyMe);
    protected :
        std::vector<InternalType *>*    getData();

    public :
        size_t                             size_get();

        void                            whoAmI(void) { std::cout << "types::List"; };

        RealType                        getType(void) { return RealList; }

        /**
        ** append(InternalType *_typedValue)
        ** Append the given value to the end of the List
        */
        void                            append(InternalType *_typedValue);

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        List*                           clone();

        wstring                         toString(int _iPrecision, int _iLineLen);

        List*                           getAsList(void) { return this; }

        bool                            insert(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, vector<types::InternalType*>* _poSource, bool _bAsVector);
        std::vector<InternalType*>      extract(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring                 getTypeStr() {return L"list";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring                 getShortTypeStr() {return L"l";}

    protected :
        std::vector<InternalType *>*    m_plData;
    };
}

#endif /* __LIST_HH__ */
