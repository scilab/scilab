/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
    Library* clone();
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
