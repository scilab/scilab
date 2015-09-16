/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LIBRARY_HXX__
#define __LIBRARY_HXX__

#include <list>
#include <unordered_map>

#include "typesdecl.hxx"

namespace types
{
class EXTERN_AST Library : public GenericType
{
public :
    Library(const std::wstring& _wstPath);
    ~Library();

    bool isLibrary(void)
    {
        return true;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr()
    {
        return L"library";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr()
    {
        return L"f";
    }

    inline ScilabType getType(void)
    {
        return ScilabLibrary;
    }
    inline ScilabId getId(void)
    {
        return IdLibrary;
    }

    bool toString(std::wostringstream& ostr);
    InternalType* clone();
    bool isAssignable()
    {
        return true;
    }

    bool extract(const std::wstring& name, InternalType *& out);

    void add(const std::wstring& _wstName, MacroFile* _macro);
    MacroFile* get(const std::wstring& _wstName);
    int getMacrosName(std::list<std::wstring>& lst);
    std::wstring getPath();
private:
    std::wstring m_wstPath;
    typedef std::unordered_map<std::wstring, MacroFile*> MacroMap;
    MacroMap m_macros;
};
}

#endif /* __LIBRARY_HXX__ */
