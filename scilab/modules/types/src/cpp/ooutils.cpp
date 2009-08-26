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

#include "ooutils.hxx"
#include "ooutils-private.hxx"
#include "string.hxx"
#include "context.hxx"

namespace types
{
  /** Public utils */
  static Function::ReturnValue
  ro_setter_func(typed_list &, int *, typed_list &out)
  {
    throw std::string("Read-only property");
  }
  
  Function *ro_setter = new Function("", ro_setter_func, "");

  Function::ReturnValue
  Method::call(typed_list &in, int* _piRetCount, typed_list &out)
  {
    symbol::Context *ctx = symbol::Context::getInstance();
    ObjectMatrix *thisref = dynamic_cast<ObjectMatrix*>(ctx->get(symbol::Symbol("this")));
    ObjectMatrix *superref = dynamic_cast<ObjectMatrix*>(ctx->get(symbol::Symbol("super")));
    return m_callback(in, _piRetCount, out, thisref, superref);
  }
  
  /** Bound method, object and setter */
  BoundMethod::BoundMethod(Function *p_func, Object *p_self, Object *p_level)
    : m_func(p_func), m_this(NULL), m_super(NULL)
  {
    if(p_level == NULL)
      p_level = p_self;
    m_this = ObjectMatrix::create_this(p_self, p_level);
    m_super = ObjectMatrix::create_super(p_self, p_level->super());
  }
  
  BoundMethod::~BoundMethod()
  {
    delete m_this;
    delete m_super;
  }
  
  Function::ReturnValue
  BoundMethod::call(typed_list &in, int* _piRetCount, typed_list &out)
  {
    Function::ReturnValue ret;
    symbol::Context *ctx = symbol::Context::getInstance();
    
    ctx->scope_begin();
    ctx->put(symbol::Symbol("this"), *m_this);
    ctx->put(symbol::Symbol("super"), *m_super);
    
    ret = inner_call(in, _piRetCount, out);
    
    ctx->scope_end();
    
    return ret;
  }
  
  Function::ReturnValue
  BoundGetter::inner_call(typed_list &in, int *_piRetCount, typed_list &out)
  {
    // push value = slot->value
    return inner_call(in, _piRetCount, out);
  }
  
  Function::ReturnValue
  BoundSetter::inner_call(typed_list &in, int *_piRetCount, typed_list &out)
  {
    Function::ReturnValue ret;
    // push value = slot->value
    ret = inner_call(in, _piRetCount, out);
    // Retrieve value and put it to slot->value
    return ret;
  }
  
  /** Root object */
  Function::ReturnValue
  install_property(typed_list &in, int *, typed_list &, ObjectMatrix *self, ObjectMatrix *)
  {
    String *name = dynamic_cast<String*>(in[0]);
    String *visibility = NULL;
    InternalType *def = NULL;
    Function *getter = NULL;
    Function *setter = NULL;
    
    if(in.size() > 1)
      visibility = dynamic_cast<String*>(in[1]);
    if(in.size() > 2)
      def = in[2];
    if(in.size() > 3)
      getter = dynamic_cast<Function*>(in[3]);
    if(in.size() > 4)
      setter = dynamic_cast<Function*>(in[4]);
    
    Slot::Visibility svisib = Slot::PUBLIC;
    if(visibility != NULL)
    {
      if(!strcmp(visibility->string_get(0,0), "public"))
        svisib = Slot::PUBLIC;
      else if(!strcmp(visibility->string_get(0,0), "protected"))
        svisib = Slot::PROTECTED;
      else if(!strcmp(visibility->string_get(0,0), "private"))
        svisib = Slot::PRIVATE;
    }
    
    self->object_ref_get()->install_property(name->string_get(0,0), svisib, def, getter, setter);
    
    return Function::AllGood;
  }
  
  Function::ReturnValue
  install_method(typed_list &in, int *, typed_list &, ObjectMatrix *self, ObjectMatrix *)
  {
    String *name = dynamic_cast<String*>(in[0]);
    String *visibility = NULL;
    Function *func = NULL;
    
    if(in.size() > 1)
      visibility = dynamic_cast<String*>(in[1]);
    if(in.size() > 2)
      func = dynamic_cast<Function*>(in[2]);
    
    Slot::Visibility svisib = Slot::PUBLIC;
    if(visibility != NULL)
    {
      if(!strcmp(visibility->string_get(0,0), "public"))
        svisib = Slot::PUBLIC;
      else if(!strcmp(visibility->string_get(0,0), "protected"))
        svisib = Slot::PROTECTED;
      else if(!strcmp(visibility->string_get(0,0), "private"))
        svisib = Slot::PRIVATE;
    }
    
    self->object_ref_get()->install_method(name->string_get(0,0), svisib, func);
    
    return Function::AllGood;
  }
}
