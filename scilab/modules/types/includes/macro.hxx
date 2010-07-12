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
#include "symbol.hxx"
#include "seqexp.hxx"

using namespace std;
namespace types
{
    class Macro : public Callable
    {
    public :
                                    Macro(): Callable(){};
                                    Macro(const wstring& _stName, list<wstring> &_inputArgs, list<wstring> &_outputArgs, ast::SeqExp &_body, const wstring& _stModule);
        virtual                     ~Macro();

        // FIXME : Should not return NULL;
        Macro*                      clone();

        Macro*                      getAsMacro(void);
        RealType                    getType(void);

        void                        whoAmI();

        wstring                     toString(int _iPrecision, int _iLineLen);

        Callable::ReturnValue       call(typed_list &in, int _iRetCount, typed_list &out, ast::ConstVisitor* execFunc);

        ast::SeqExp*                body_get();

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring             getTypeStr() {return L"macro";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring             getShortTypeStr() {return L"function";}

        list<wstring>*              inputs_get();
        list<wstring>*              outputs_get();

    private :
        list<wstring>*              m_inputArgs;
        list<wstring>*              m_outputArgs;
        ast::SeqExp*                m_body;
        bool                        bAutoAlloc;

    };
}


#endif /* !__MACRO_HXX__ */
