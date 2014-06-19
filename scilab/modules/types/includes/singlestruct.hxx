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


#ifndef __SINGLESTRUCT_HXX__
#define __SINGLESTRUCT_HXX__

#include <map>
#include <list>
#include "internal.hxx"
#include "dynlib_types.h"
#include "string.hxx"

namespace types
{
class TYPES_IMPEXP SingleStruct : public InternalType
{
public :
    SingleStruct();
    ~SingleStruct();

private :
    SingleStruct(SingleStruct *_oListCopyMe);

public :
    SingleStruct*                           clone();

    inline ScilabType                       getType(void)
    {
        return ScilabSingleStruct;
    }
    inline ScilabId                         getId(void)
    {
        return IdSingleStruct;
    }

    bool                                    isSingleStruct()
    {
        return true;
    }

    bool                                    toString(std::wostringstream& ostr);

    bool                                    set(const std::wstring& _sKey, InternalType *_typedValue);
    InternalType*                           get(const std::wstring& _sKey);
    bool                                    exists(const std::wstring& _sKey);
    InternalType*                           insert(typed_list* _pArgs, InternalType* _pSource);
    std::vector<InternalType*>              extract(std::list<std::wstring> _stFields);
    String*                                 getFieldNames();
    bool                                    addField(const std::wstring& _sKey);
    bool                                    addFieldFront(const std::wstring& _sKey);
    std::list<InternalType *>               getData();
    std::list<std::wstring>                 getFields();
    int                                     getFieldIndex(const std::wstring& _field);
    bool                                    removeField(const std::wstring& _sKey);
    void                                    addFieldsFront(String* _pStrFieldNames);

    bool                                    operator==(const InternalType& it);
    bool                                    operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring                    getTypeStr()
    {
        return L"";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring                    getShortTypeStr()
    {
        return L"";
    }

private :
    std::list<std::wstring> m_wstFields;
    std::list<InternalType *> m_Data;
};
}

#endif /* !__SINGLESTRUCT_HXX__ */
