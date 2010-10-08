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


#ifndef __STRUCT_HXX__
#define __STRUCT_HXX__

#include <map>
#include "container.hxx"
#include "symbol.hxx"

namespace types
{
    class Struct : public Container
    {
    public :
                                                Struct();
                                                ~Struct(); 

    private :
                                                Struct(Struct *_oListCopyMe);
        std::map<std::wstring, InternalType *>* getData();

    public :
        int                                     size_get(); 

        void                                    whoAmI(void) { std::cout << "types::Struct"; };

        RealType                                getType(void) { return RealStruct; } 

        /**
        ** add(Symbol *_psKey, InternalType *_typedValue)
        ** Append the given value to the struct
        */
        void                                    add(const std::wstring& _sKey, InternalType *_typedValue);

        /**
        ** add(Symbol *_psKey)
        ** Append an null value to the struct
        */
        void                                    add(const std::wstring& _sKey);

        /**
        ** get(Symbol *_psKey)
        ** Append the given value to the end of the List
        */
        InternalType*                           get(const std::wstring& _sKey);

        /**
        **
        */
        bool                                    exists(const std::wstring& _sKey);

        std::vector<InternalType*>              extract(list<wstring> _stFields);

        /**
        ** Clone
        ** Create a new List and Copy all values.
        */
        Struct*                                 clone();

        wstring                                 toString(int _iPrecision, int _iLineLen);

        Struct*                                 getAsStruct(void) { return this; }

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring                         getTypeStr() {return L"struct";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring                         getShortTypeStr() {return L"st";}
    private :
        std::map<std::wstring, InternalType *>*  m_plData;
    };
}

#endif /* !__STRUCT_HXX__ */
