/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
        m_Nargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol("nargin"))),
        m_Nargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol("nargout"))),
        m_Varargin(symbol::Context::getInstance()->getOrCreate(symbol::Symbol("varargin"))),
        m_Varargout(symbol::Context::getInstance()->getOrCreate(symbol::Symbol("varargout")))
    {
    }

    Macro(const std::string& _name, std::list<symbol::Variable*> &_inputArgs, std::list<symbol::Variable*> &_outputArgs, ast::SeqExp &_body, const std::string& _module);
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

    bool                        toString(std::ostringstream& ostr) override;

    Callable::ReturnValue       call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out) override;

    inline void cleanCall(symbol::Context * pContext, int oldPromptMode);

    ast::SeqExp*                getBody();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string        getTypeStr()
    {
        return "function";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::string        getShortTypeStr()
    {
        return "function";
    }

    const std::string&         getFileName()
    {
        return m_stPath;
    }

    void                        setFileName(const std::string& _fileName)
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
    std::string                     m_stPath;
};
}


#endif /* !__MACRO_HXX__ */
