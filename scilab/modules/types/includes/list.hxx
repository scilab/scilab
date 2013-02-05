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
#include "dynlib_types.h"

namespace types
{
class TYPES_IMPEXP List : public Container
{
public :
    List();
    ~List();

private :
    List(List *_oListCopyMe);
protected :
    std::vector<InternalType *>*    getData();
public :
    int                             getSize();

    void                            whoAmI(void)
    {
        std::cout << "types::List";
    };

    RealType                        getType(void)
    {
        return RealList;
    }

    /**
    ** append(InternalType *_typedValue)
    ** Append the given value to the end of the List
    */
    void                            append(InternalType *_typedValue);

    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    InternalType*                   clone();

    GenericType*                    getColumnValues(int _iPos);

    bool                            toString(std::wostringstream& ostr);

    bool                            isList()
    {
        return true;
    }

    InternalType*                   insert(typed_list* _pArgs, InternalType* _pSource);
    std::vector<InternalType*>      extract(typed_list* _pArgs);
    virtual InternalType*           get(const int _iIndex);
    virtual bool                    set(const int _iIndex, InternalType* _pIT);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring            getTypeStr()
    {
        return L"list";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring            getShortTypeStr()
    {
        return L"l";
    }

    virtual bool                    operator==(const InternalType& it);

protected :
    std::vector<InternalType *>*    m_plData;
};
}

#endif /* __LIST_HH__ */
