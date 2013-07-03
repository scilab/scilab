/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __VARIABLES_HXX__
#define __VARIABLES_HXX__

#include <map>
#include <list>
#include "types.hxx"
#include "symbol.hxx"

namespace symbol
{
struct VarBox
{
    VarBox(int _iLevel, types::InternalType* _pIT, bool _bGlobal = false, bool _bGlobalVisible = false);

    int m_iLevel;
    types::InternalType* m_pIT;
    bool m_bGlobal;
    bool m_bGlobalVisible;
};

class Variables
{
private :
    //current scope level
    int m_iLevel;

    typedef std::list<VarBox* > VarBoxList;
    typedef std::map<Symbol, VarBoxList*> VarMap;
    typedef std::map<Symbol, types::InternalType*> GlobalMap;
    VarMap m_vars;
    GlobalMap m_globals;

    void removeGlobal(GlobalMap::iterator& _it);
public :
    Variables() : m_iLevel(-1) {}

    virtual ~Variables() {}

    void put(const Symbol& _key, types::InternalType& _pIT);
    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
    void putInPreviousScope(const Symbol& _key, types::InternalType& _iT);

    types::InternalType* get(const Symbol& _key) const;
    types::InternalType* getCurrentLevel(const Symbol& _key) const;
    types::InternalType* getAllButCurrentLevel(const Symbol& _key) const;
    types::InternalType* getInSpecificLevel(const Symbol& _key, int _iLevel) const;

    std::list<symbol::Symbol>& getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd) const;
    std::list<std::wstring>& getVarsName();
    std::list<std::wstring>& getMacrosName();
    std::list<std::wstring>& getFunctionsName();

    /*globals*/
    bool isGlobalVisible(const symbol::Symbol& _key) const;
    void setGlobalVisible(const symbol::Symbol& _key, bool bVisible);
    void removeGlobal(const symbol::Symbol& _key);
    void removeGlobalAll();
    void createEmptyGlobalValue(const symbol::Symbol& _key);
    bool isGlobalExists(const symbol::Symbol& _key) const;
    types::InternalType* getGlobalValue(const symbol::Symbol& _key) const;
    void setGlobalValue(const symbol::Symbol& _key, types::InternalType& _value);

    bool remove(const Symbol& _key);

    void IncreaseLevel()
    {
        m_iLevel++;
    }
    void DecreaseLevel()
    {
        m_iLevel--;
    }
};
}
#endif /* !__VARIABLES_HXX__ */
