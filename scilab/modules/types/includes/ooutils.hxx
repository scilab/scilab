/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Simon LIPP
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __OOUTILS_HXX__
#define __OOUTILS_HXX__

#include "objectmatrix.hxx"

namespace types
{
  class Method: public Function
  {
  public:
    typedef Function::ReturnValue (*GW_METH)(typed_list &in, int iRetCount, typed_list &out,
                                             ObjectMatrix *p_this, ObjectMatrix *p_super);
  
    Function::ReturnValue call(typed_list &in, int iRetCount, typed_list &out);
  
    Method(GW_METH p_callback): Function(), m_callback(p_callback) {}
    virtual ~Method() {}
  
  protected:
    GW_METH m_callback;
  };
  
  extern Function *ro_setter;
}

#endif /* !__OOUTILS_HXX__ */
