/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __MACRO_HXX__
#define __MACRO_HXX__

#include <list>
#include <string>
#include "context.hxx"
#include "types.hxx"
#include "callable.hxx"
#include "double.hxx"
#include "seqexp.hxx"

namespace types
{
class EXTERN_AST Macro : public Callable
{
public :
    Macro() : Callable(),
        m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargin"))),
        m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"nargout"))),
        m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargin"))),
        m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol(L"varargout")))
    {
    }

    Macro(const std::wstring& _stName, std::list<symbol::Variable*> &_inputArgs, std::list<symbol::Variable*> &_outputArgs, ast::SeqExp &_body, const std::wstring& _stModule);
    virtual                     ~Macro();

    // FIXME : Should not return NULL;
    Macro*                      clone();

    inline ScilabType           getType(void)
    {
        return ScilabMacro;
    }
    inline ScilabId             getId(void)
    {
        return IdMacro;
    }
    bool                        isMacro()
    {
        return true;
    }

    void                        whoAmI();

    bool                        toString(std::wostringstream& ostr);

    Callable::ReturnValue       call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;

    inline void cleanCall(symbol::Context * pContext, int oldPromptMode);

    ast::SeqExp*                getBody();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring        getTypeStr()
    {
        return L"function";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr()
    {
        return L"function";
    }

    const std::wstring&         getFileName()
    {
        return m_stPath;
    }

    void                        setFileName(const std::wstring& _fileName)
    {
        m_stPath = _fileName;
    }

    std::list<symbol::Variable*>*   getInputs();
    std::list<symbol::Variable*>*   getOutputs();

    virtual int getNbInputArgument(void);
    virtual int getNbOutputArgument(void);

    bool operator==(const InternalType& it);

    void add_submacro(const symbol::Symbol& s, Macro* macro);

    inline const std::map<symbol::Variable*, Macro*> & getSubMacros() const
    {
        return m_submacro;
    }

    inline const std::map<symbol::Variable*, Macro*> & getSubMacros()
    {
        return m_submacro;
    }

private :
    std::list<symbol::Variable*>*   m_inputArgs;
    std::list<symbol::Variable*>*   m_outputArgs;
    ast::SeqExp*                    m_body;
    bool                            bAutoAlloc;
    symbol::Variable*               m_Nargin;
    symbol::Variable*               m_Nargout;
    symbol::Variable*               m_Varargin;
    symbol::Variable*               m_Varargout;
    Double*                         m_pDblArgIn;
    Double*                         m_pDblArgOut;
    std::map<symbol::Variable*, Macro*> m_submacro;
    std::wstring                    m_stPath;
};
}


#endif /* !__MACRO_HXX__ */
