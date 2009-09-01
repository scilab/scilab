/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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
#include "callable.hxx"
#include "types.hxx"
#include "symbol.hxx"
#include "seqexp.hxx"

namespace types
{
  class EXTERN_TYPES Macro : public Callable
  {
  public :
		Macro *getAsMacro(void);
    RealType getType(void);

    void whoAmI();

		Macro(): Callable(){};
		Macro(std::string _stName, std::list<symbol::Symbol> &_inputArgs, std::list<symbol::Symbol> &_outputArgs, ast::SeqExp &_body);
		virtual ~Macro(){};
                ReturnValue call(typed_list &in, int _iRetCount, typed_list &out);
  
  public :
    std::list<symbol::Symbol>	*m_inputArgs;
    std::list<symbol::Symbol>	*m_outputArgs;
    ast::SeqExp			*m_body;
    std::string			m_stName;
  };
}


#endif /* !__MACRO_HXX__ */
