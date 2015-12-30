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

#include <unordered_map>
#include <vector>

#include "internal.hxx"
#include "string.hxx"

namespace types
{
class EXTERN_AST SingleStruct : public InternalType
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

    inline int getNumFields() const
    {
        return static_cast<int>(m_stFields.size());
    }

    bool                                    toString(std::ostringstream& ostr) override;

    bool                                    set(const std::string& _sKey, InternalType *_typedValue);
    InternalType*                           get(const std::string& _sKey);
    bool                                    exists(const std::string& _sKey);
    InternalType*                           insert(typed_list* _pArgs, InternalType* _pSource);
    std::vector<InternalType*>              extract(std::vector<std::string> & _stFields);
    String*                                 getFieldNames();
    bool                                    addField(const std::string& _sKey);
    bool                                    addFieldFront(const std::string& _sKey);
    std::vector<InternalType *> &           getData();
    std::unordered_map<std::string, int> & getFields();
    int                                     getFieldIndex(const std::string& _field);
    bool                                    removeField(const std::string& _sKey);

    bool                                    operator==(const InternalType& it);
    bool                                    operator!=(const InternalType& it);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string                    getTypeStr()
    {
        return "singlestruct";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string                    getShortTypeStr()
    {
        return "sst";
    }

private :

    std::unordered_map<std::string, int> m_stFields;
    std::vector<InternalType *> m_Data;
};
}

#endif /* !__SINGLESTRUCT_HXX__ */
