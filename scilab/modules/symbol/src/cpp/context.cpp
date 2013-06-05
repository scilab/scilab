/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "context.hxx"
#include "function.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "addGatewayInContext.h"

void addGatewayInContext(wchar_t* _pwstName, OLDGW_FUNC _pFunc, wchar_t* _pwstModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_pwstName, _pFunc, _pwstModule));
}

void addMexGatewayInContext(wchar_t* _pwstName, MEXGW_FUNC _pFunc, wchar_t* _pwstModule)
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(_pwstName, _pFunc, _pwstModule));
}

namespace symbol
{
Context* Context::me;

Context::Context()
{
    m_scopes = new Scopes();
    m_scopes->scope_begin();
}

Context* Context::getInstance(void)
{
    if (me == 0)
    {
        me = new Context();
    }
    return me;
}

void Context::scope_begin()
{
    m_scopes->scope_begin();
}

void Context::scope_end()
{
    m_scopes->scope_end();
}

types::InternalType* Context::get(const symbol::Symbol& _key) const
{
    return m_scopes->get(_key);
}

types::InternalType* Context::getCurrentLevel(const symbol::Symbol& _key) const
{
    return m_scopes->getCurrentLevel(_key);
}

types::InternalType* Context::getAllButCurrentLevel(const symbol::Symbol& _key) const
{
    return m_scopes->getAllButCurrentLevel(_key);
}

types::InternalType* Context::getFunction(const symbol::Symbol& _key) const
{
    return m_scopes->get(_key);
}

std::list<symbol::Symbol>& Context::getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd)
{
    return m_scopes->getFunctionList(_stModuleName, _bFromEnd);
}

bool Context::put(const symbol::Symbol& _key, types::InternalType &type)
{
    m_scopes->put(_key, type);
    return true;
}

bool Context::remove(const symbol::Symbol& _key)
{
    m_scopes->remove(_key);
    return true;
}

bool Context::putInPreviousScope(const symbol::Symbol& _key, types::InternalType &type)
{
    m_scopes->putInPreviousScope(_key, type);
    return true;
}

bool Context::addFunction(types::Function *_info)
{
    m_scopes->put(symbol::Symbol(_info->getName()), *_info);
    return true;
}

bool Context::AddMacro(types::Macro *_info)
{
    m_scopes->put(symbol::Symbol(_info->getName()), *_info);
    return true;
}

bool Context::AddMacroFile(types::MacroFile *_info)
{
    m_scopes->put(symbol::Symbol(_info->getName()), *_info);
    return true;
}

bool Context::isGlobalVisible(const symbol::Symbol& _key) const
{
    return m_scopes->isGlobalVisible(_key);
}

/*return global variable, search in global scope ( highest )*/
types::InternalType* Context::getGlobalValue(const symbol::Symbol& _key) const
{
    m_scopes->getGlobalValue(_key);
    return NULL;
}

/*return global variable existance status*/
bool Context::isGlobalExists(const symbol::Symbol& _key) const
{
    return m_scopes->isGlobalExists(_key);
}

void Context::setGlobalValue(const symbol::Symbol& _key, types::InternalType &value)
{
    m_scopes->setGlobalValue(_key, value);
}

void Context::createEmptyGlobalValue(const symbol::Symbol& _key)
{
    m_scopes->createEmptyGlobalValue(_key);
}

void Context::setGlobalVisible(const symbol::Symbol& _key, bool bVisible)
{
    m_scopes->setGlobalVisible(_key, bVisible);
}

void Context::removeGlobal(const symbol::Symbol& _key)
{
    m_scopes->removeGlobal(_key);
}

void Context::removeGlobalAll()
{
    m_scopes->removeGlobalAll();
}
}
