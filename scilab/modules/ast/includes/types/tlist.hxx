/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TLIST_HXX__
#define __TLIST_HXX__

#include <list>
#include <vector>
#include "list.hxx"
#include "string.hxx"

namespace types
{
class EXTERN_AST TList : public List
{
public :
    TList();
    ~TList();
protected :
    TList(TList *_oTListCopyMe) : List(_oTListCopyMe) {}

public :
    void                            whoAmI(void)
    {
        std::cout << "types::TList";
    }

    ScilabType                      getType(void)
    {
        return ScilabTList;
    }

    bool                            isTList()
    {
        return true;
    }

    virtual TList*                  clone();

    bool                            exists(const std::string& _sKey);
    InternalType*                   getField(const std::string& _sKey);
    int                             getIndexFromString(const std::string& _sKey);
    TList*                          set(const std::string& _sKey, InternalType* _pIT);
    TList*                          set(const int _iIndex, InternalType* _pIT);

    using List::extract; // to avoid this extract to hide extract in list
    bool                            extract(const std::string& name, InternalType *& out);

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e) override;

    bool isFieldExtractionOverloadable() const
    {
        return true;
    }

    InternalType*                   extractStrings(const std::list<std::string>& _stFields);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string            getTypeStr();
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string            getShortTypeStr();

    String*                         getFieldNames();

    bool                            toString(std::ostringstream& ostr) override;

private :
};
}

#endif /* __TLIST_HXX__ */
