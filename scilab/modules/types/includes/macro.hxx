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
#include "types.hxx"
#include "callable.hxx"
#include "double.hxx"
#include "seqexp.hxx"

using namespace std;
namespace types
{
class Macro : public Callable
{
public :
    Macro() : Callable(), m_ArgInSymb(symbol::Symbol(L"nargin")), m_ArgOutSymb(symbol::Symbol(L"nargout")) {}
    Macro(const std::wstring& _stName, std::list<symbol::Symbol> &_inputArgs, std::list<symbol::Symbol> &_outputArgs, ast::SeqExp &_body, const wstring& _stModule);
    virtual                     ~Macro();

    // FIXME : Should not return NULL;
    InternalType*               clone();

    RealType                    getType(void);
    bool                        isMacro()
    {
        return true;
    }

    void                        whoAmI();

    bool                        toString(std::wostringstream& ostr);

    Callable::ReturnValue       call(typed_list &in, optional_list &opt, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

    ast::SeqExp*                getBody();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual wstring             getTypeStr()
    {
        return L"function";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual wstring             getShortTypeStr()
    {
        return L"function";
    }

    list<symbol::Symbol>*       inputs_get();
    list<symbol::Symbol>*       outputs_get();

private :
    std::list<symbol::Symbol>*  m_inputArgs;
    std::list<symbol::Symbol>*  m_outputArgs;
    ast::SeqExp*                m_body;
    bool                        bAutoAlloc;
    symbol::Symbol              m_ArgInSymb;
    symbol::Symbol              m_ArgOutSymb;
    Double*                     m_pDblArgIn;
    Double*                     m_pDblArgOut;

};
}


#endif /* !__MACRO_HXX__ */
