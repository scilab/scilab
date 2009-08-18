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
#include "types.hxx"
#include "symbol.hxx"
#include "seqexp.hxx"
#include "function.hxx"

namespace types
{
  class EXTERN_TYPES Macro : public Function
  {
  public :
    Macro *getAsMacro(void) { return this; }
    RealType getType(void) { return RealMacro; }

    void whoAmI();
    virtual ReturnValue call(typed_list &in, int* _piRetCount, typed_list &out);

    Macro() {};
    Macro(std::list<symbol::Symbol> &_inputArgs, std::list<symbol::Symbol> &_outputArgs, ast::Exp &_body) :
      m_inputArgs(&_inputArgs),
      m_outputArgs(&_outputArgs),
      m_body(&_body)
      {
      };
    virtual ~Macro()
    {
      delete m_inputArgs;
      delete m_outputArgs;
      delete m_body;
    }
  
  public :
    std::list<symbol::Symbol>	*m_inputArgs;
    std::list<symbol::Symbol>	*m_outputArgs;
    ast::Exp			*m_body;
  };
}


#endif /* !__MACRO_HXX__ */
