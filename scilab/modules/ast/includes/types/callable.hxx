/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

    void setName(const std::string& _stName)
    {
        m_stName = _stName;
    }
    const std::string& getName()
    {
        return m_stName;
    }
    void setModule(const std::string& _stModule)
    {
        m_stModule = _stModule;
    }
    std::string getModule()
    {
        return m_stModule;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string  getTypeStr()
    {
        return "callable";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string  getShortTypeStr() = 0;
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
    std::string             m_stName;
    std::string             m_stModule;
    int                     m_iFirstLine;
    int                     m_iLastLine;
};
}


#endif /* !__CALLABLE_HXX__ */
