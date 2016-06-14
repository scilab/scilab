/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __CALLABLE_HXX__
#define __CALLABLE_HXX__

#include <string>
#include "internal.hxx"

#include "visitor.hxx"

//disable warnings about exports STL items
#ifdef _MSC_VER
#pragma warning (disable : 4251)
#endif

namespace types
{
class EXTERN_AST Callable : public InternalType
{
public :
    enum ReturnValue
    {
        OK,
        OK_NoResult,
        Error
    };

    Callable(): InternalType()
    {
        m_iFirstLine = 0;
        m_iLastLine = 0;
    }
    virtual             ~Callable() {}

    bool                isCallable()
    {
        return true;
    }

    virtual ReturnValue call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) = 0;

    virtual bool invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, const ast::Exp & e)  override;

    virtual bool isInvokable() const
    {
        return true;
    }

    virtual bool hasInvokeOption() const
    {
        return true;
    }

    virtual int getInvokeNbIn()
    {
        return getNbInputArgument();
    }

    virtual int getInvokeNbOut()
    {
        return getNbOutputArgument();
    }

    void                  setName(const std::wstring& _wstName)
    {
        m_wstName = _wstName;
    }
    const std::wstring&   getName()
    {
        return m_wstName;
    }
    void                  setModule(const std::wstring& _wstModule)
    {
        m_wstModule = _wstModule;
    }
    std::wstring        getModule()
    {
        return m_wstModule;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring  getTypeStr()
    {
        return L"callable";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring  getShortTypeStr() = 0;
    virtual InternalType* clone(void) = 0;

    virtual bool        isAssignable(void)
    {
        return true;
    }

    virtual int        getNbInputArgument(void)
    {
        return -1;
    }

    virtual int        getNbOutputArgument(void)
    {
        return -1;
    }

    virtual int        getFirstLine(void)
    {
        return m_iFirstLine;
    }

    virtual int        getLastLine(void)
    {
        return m_iLastLine;
    }

    virtual void       setLines(int _iFirstLine, int _iLastLine)
    {
        m_iFirstLine = _iFirstLine;
        m_iLastLine  = _iLastLine;
    }

protected :
    std::wstring            m_wstName;
    std::string             m_stName;
    std::wstring            m_wstModule;
    int                     m_iFirstLine;
    int                     m_iLastLine;
};
}


#endif /* !__CALLABLE_HXX__ */
