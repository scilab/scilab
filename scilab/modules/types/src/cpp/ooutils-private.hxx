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
  class BoundMethod: public Callable
  {
  public:
    BoundMethod(Callable *p_func, Object *p_self, Object *p_level, ObjectMatrix *p_sender);
    virtual ~BoundMethod();
    
    Callable::ReturnValue call(typed_list &in, int iRetCount, typed_list &out);
    virtual Callable::ReturnValue inner_call(typed_list &in, int iRetCount, typed_list &out);
  
  protected:
    Callable *m_func;
    ObjectMatrix *m_this;
    ObjectMatrix *m_super;
    ObjectMatrix *m_sender;
  };
  
  class BoundGetter: public BoundMethod
  {
  public:
    BoundGetter(PropertySlot *p_slot, Object *p_self, Object *p_level, ObjectMatrix *p_sender):
      BoundMethod(p_slot->getter, p_self, p_level, p_sender), m_slot(p_slot) {}
    virtual ~BoundGetter() {}
    
    Callable::ReturnValue inner_call(typed_list &in, int iRetCount, typed_list &out);
  
  protected:
    PropertySlot *m_slot;
  };
  
  class BoundSetter: public BoundMethod
  {
  public:
    BoundSetter(PropertySlot *p_slot, Object *p_self, Object *p_level, ObjectMatrix *p_sender):
      BoundMethod(p_slot->setter, p_self, p_level, p_sender), m_slot(p_slot) {}
    virtual ~BoundSetter() {}
    
    Callable::ReturnValue inner_call(typed_list &in, int iRetCount, typed_list &out);
  
  protected:
    PropertySlot *m_slot;
  };
  
  /* Methods of root object */
  Callable::ReturnValue install_property(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
  Callable::ReturnValue install_method(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
  Callable::ReturnValue object_get(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
  Callable::ReturnValue object_set(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
  Callable::ReturnValue slots_list(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
  Callable::ReturnValue remove_slot(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
}

#endif /* !__OOUTILS_PRIVATE_HXX__ */
