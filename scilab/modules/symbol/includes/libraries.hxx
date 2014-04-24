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

#ifndef __LIBRARIES_HXX__
#define __LIBRARIES_HXX__

#include <map>
#include <list>
#include "types.hxx"
#include "symbol.hxx"
#include "function.hxx"
#include "library.hxx"
#include "variables.hxx"


namespace symbol
{
class LibBox
{
public :
    LibBox(int _iLevel, types::Library& _lib) : m_iLevel(_iLevel), m_pLib(&_lib) {};
    virtual ~LibBox() {};

    types::Library* getValue()
    {
        return m_pLib;
    }
    types::MacroFile* getMacroFile(const Symbol& _key);
    void setValue(types::Library& _lib)
    {
        m_pLib = &_lib;
    }
    int getLevel()
    {
        return m_iLevel;
    };
    void setLevel(int _iLevel)
    {
        m_iLevel = _iLevel;
    };
private :
    int m_iLevel;
    types::Library* m_pLib;
};

class Libraries
{
private :
    typedef std::list<LibBox* > LibBoxList;
    typedef std::map<Symbol, LibBoxList*> LibMap;
    typedef std::list<Symbol> LibList;
    LibMap m_libmap;
    LibList m_liblist;
    int m_iLevel;

public :
    Libraries() : m_iLevel(-1) {}

    virtual ~Libraries() {}

    void IncreaseLevel()
    {
        m_iLevel++;
    }
    void DecreaseLevel()
    {
        m_iLevel--;
    }

    void put(const Symbol& _key, types::InternalType& _IT);
    types::MacroFile* get(const Symbol& _key) const;
    bool remove(const Symbol& _key);

    std::list<std::wstring>* getMacrosName();

};
}
#endif /* !__LIBRARIES_HXX__ */
