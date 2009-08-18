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

#include "macro.hxx"
#include "context.hxx"
#include "execvisitor.hxx"

namespace types
{
  /*--------------*/
  /*	whoIAm	  */
  /*--------------*/
  void Macro::whoAmI()
  {
    std::cout << "types::Macro";
  }
  
  Function::ReturnValue Macro::call(typed_list &in, int* _piRetCount, typed_list &out)
  {
    // FIXME: what to do with _piRetCount ?
   
    if(in.size() != m_inputArgs->size())
    {
      return Function::WrongParamNumber;
    }
    
    symbol::Context *ctx = symbol::Context::getInstance();
    ctx->scope_begin();
    
    // Copy input arguments to the stack
    {
      std::list<symbol::Symbol>::iterator it_argname = m_inputArgs->begin();
      typed_list::iterator it_argval = in.begin();
      for( ; it_argname != m_inputArgs->end(); ++it_argname, ++it_argval)
      {
        ctx->put(*it_argname, *(*it_argval)->clone());
      }
    }
    
    // Call the code
    ast::ExecVisitor execMe;
    m_body->accept(execMe);
    
    // Get ouput arguments from the stack
    {
      std::list<symbol::Symbol>::iterator it_argname = m_outputArgs->begin();
      for( ; it_argname != m_outputArgs->end(); ++it_argname)
      {
        out.push_back(ctx->get(*it_argname)->clone());
      }
    }
    
    ctx->scope_end();
    
    return Function::AllGood;
  }
}
