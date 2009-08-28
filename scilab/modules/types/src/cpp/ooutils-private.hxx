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

#ifndef __OOUTILS_PRIVATE_HXX__
#define __OOUTILS_PRIVATE_HXX__

#include "object.hxx"

namespace types
{
  class BoundMethod: public Function
  {
  public:
    BoundMethod(Function *p_func, Object *p_self, Object *p_level = NULL);
    virtual ~BoundMethod();
    
    Function::ReturnValue call(typed_list &in, int iRetCount, typed_list &out);
    virtual Function::ReturnValue inner_call(typed_list &in, int iRetCount, typed_list &out) {
      return m_func->call(in, iRetCount, out);
    }
  
  protected:
    Function *m_func;
    ObjectMatrix *m_this;
    ObjectMatrix *m_super;
  };
  
  class BoundGetter: public BoundMethod
  {
  public:
    BoundGetter(PropertySlot *p_slot, Object *p_self, Object *p_level):
      BoundMethod(p_slot->getter, p_self, p_level), m_slot(p_slot) {}
    virtual ~BoundGetter();
    
    Function::ReturnValue inner_call(typed_list &in, int iRetCount, typed_list &out);
  
  protected:
    PropertySlot *m_slot;
  };
  
  class BoundSetter: public BoundMethod
  {
  public:
    BoundSetter(PropertySlot *p_slot, Object *p_self, Object *p_level):
      BoundMethod(p_slot->setter, p_self, p_level), m_slot(p_slot) {}
    virtual ~BoundSetter();
    
    Function::ReturnValue inner_call(typed_list &in, int iRetCount, typed_list &out);
  
  protected:
    PropertySlot *m_slot;
  };
  
  /* Methods of root object */
  Function::ReturnValue install_property(typed_list&, int, typed_list&, ObjectMatrix*, ObjectMatrix*);
  Function::ReturnValue install_method(typed_list&, int, typed_list&, ObjectMatrix*, ObjectMatrix*);
}

#endif /* !__OOUTILS_PRIVATE_HXX__ */
