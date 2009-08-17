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

#include <cstdarg>

#include "object.hxx"
#include "string.hxx"
#include "context.hxx"

namespace types
{
  /**
   * Utilitarian classes
   */
  class BoundMethod: public Function
  {
  public:
    BoundMethod(Function *p_func, Object *p_self, Object *p_level = NULL);
    virtual ~BoundMethod();
    
    Function::ReturnValue call(typed_list &in, int* _piRetCount, typed_list &out);
    virtual Function::ReturnValue inner_call(typed_list &in, int* _piRetCount, typed_list &out) {
      return m_func->call(in, _piRetCount, out);
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
    
    Function::ReturnValue inner_call(typed_list &in, int *_piRetCount, typed_list &out);
  
  protected:
    PropertySlot *m_slot;
  };
  
  class BoundSetter: public BoundMethod
  {
  public:
    BoundSetter(PropertySlot *p_slot, Object *p_self, Object *p_level):
      BoundMethod(p_slot->setter, p_self, p_level), m_slot(p_slot) {}
    virtual ~BoundSetter();
    
    Function::ReturnValue inner_call(typed_list &in, int *_piRetCount, typed_list &out);
  
  protected:
    PropertySlot *m_slot;
  };
  
  BoundMethod::BoundMethod(Function *p_func, Object *p_self, Object *p_level)
    : m_func(p_func)
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
  
  /**
   * Object
   */
  void
  Object::install_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Function *p_getter, Function *p_setter)
  {
    SlotsIterator it = m_slots.find(p_slotName);
    if(it != m_slots.end())
    {
      printf("Error: slot already exists\n");
      return;
    }
    
    PropertySlot *slot = new PropertySlot;
    slot->visibility = p_visibility;
    slot->value = p_default;
    slot->getter = p_getter;
    slot->setter = p_setter;
    m_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
  }
  
  void
  Object::install_method(const std::string &p_slotName, Slot::Visibility p_visibility, Function *p_func)
  {
    SlotsIterator it = m_slots.find(p_slotName);
    if(it != m_slots.end())
    {
      printf("Error: slot already exists\n");
      return;
    }
    
    MethodSlot *slot = new MethodSlot;
    slot->visibility = p_visibility;
    slot->func = p_func;
    m_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
  }
  
  bool
  Object::resolv_slot(const std::string &p_slotName, const ObjectMatrix *p_sender,
    Object **r_lvptr, Slot **r_slot)
  {
    bool res = resolv_slot_local(p_slotName, p_sender, r_slot);
    if(*r_slot != NULL)
    {
      // Found at this level
      *r_lvptr = this;
      return true;
    }
    else
    {
      // Try upper level, or fail
      if(m_isa != NULL)
        res |= m_isa->resolv_slot(p_slotName, p_sender, r_lvptr, r_slot);
      else
        *r_lvptr = NULL;
      return res;
    }
  }

  bool
  Object::resolv_slot_local(const std::string &p_slotName, const ObjectMatrix *p_sender,
    Slot **r_slot)
  {
    SlotsIterator it = m_slots.find(p_slotName);
    *r_slot = NULL;
    if(it != m_slots.end())
    {
      if(is_visible(&it->second, p_sender))
        *r_slot = &it->second;
      return true;
    }
    return false;
  }

  bool
  Object::is_visible(const Slot *p_slot, const ObjectMatrix *p_sender) const
  {
    // Public: always visible
    if(p_slot->visibility == Slot::PUBLIC)
      return true;
    
    // Protected and private can’t be visible from a function
    if(p_sender == NULL)
      return false;
    
    // Private: visible if sender level is current object
    if(p_slot->visibility == Slot::PRIVATE)
      return p_sender->level_ref_get() == this;
    
    // Protected: visible if current object is in the sender super chain
    const Object *cur_level = p_sender->object_ref_get();
    while(cur_level)
    {
      if(cur_level == this)
        return true;
      cur_level = cur_level->m_isa;
    }
    return false;
  }
  
  InternalType *
  Object::get(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender)
  {
    Slot *slot;
    Object *level;
    
    // Standard slots
    bool res = resolv_slot(p_slotName, p_sender, &level, &slot);
    if(slot != NULL && level != NULL)
    {
      return p_self->do_get(slot, level);
    }
    else if(res)
    {
      printf("Error: slot %s found but not visible from this context\n", p_slotName.c_str());
      return NULL;
    }
    
    // Not found: try to call a magic method
    resolv_slot("%get_slot", NULL, &level, &slot);
    if(slot != NULL && level != NULL)
    {
      MethodSlot *magic = dynamic_cast<MethodSlot*>(slot);
      if(magic)
        return p_self->do_call(magic->func, level, new String(p_slotName.c_str()), NULL);
    }
    
    // No magic method, error
    fprintf(stderr, "Error: slot %s not found\n", p_slotName.c_str());
    return NULL;
  }
  
  InternalType *
  Object::get_priv(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender)
  {
    Slot *slot;
    resolv_slot_local(p_slotName, p_sender, &slot);
    if(slot != NULL)
      if(slot->visibility == Slot::PRIVATE)
        return p_self->do_get(slot, this);
    return NULL;
  }

  void
  Object::set(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender)
  {
    Slot *slot;
    Object *level;
    
    // Standard slots
    bool res = resolv_slot(p_slotName, p_sender, &level, &slot);
    if(slot != NULL && level != NULL)
    {
      p_self->do_set(slot, level, p_value);
      return;
    }
    else if(res)
    {
      printf("Error: slot %s found but not visible from this context\n", p_slotName.c_str());
      return;
    }
    
    // Not found: try to call a magic method
    resolv_slot("%set_slot", NULL, &level, &slot);
    if(slot != NULL && level != NULL)
    {
      MethodSlot *magic = dynamic_cast<MethodSlot*>(slot);
      if(magic)
      {
        p_self->do_call(magic->func, level, new String(p_slotName.c_str()), p_value, NULL);
        return;
      }
    }
    
    // No magic method, error
    printf("Error: slot %s not found\n", p_slotName.c_str());
  }
  
  bool
  Object::set_priv(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender)
  {
    Slot *slot;
    resolv_slot_local(p_slotName, p_sender, &slot);
    if(slot != NULL)
    {
      if(slot->visibility == Slot::PRIVATE)
      {
        p_self->do_set(slot, this, p_value);
        return true;
      }
    }
    return false;
  }
  
  InternalType *
  Object::do_get(Slot *p_slot, Object *p_level)
  {
    MethodSlot *meth = dynamic_cast<MethodSlot*>(p_slot);
    if(meth == NULL)
    {
      PropertySlot *prop = dynamic_cast<PropertySlot*>(p_slot);
      if(prop->getter)
        return do_call(prop->getter, p_level, NULL);
      else
        return prop->value;
    }
    else
    {
      return do_bind(meth->func, p_level);
    }
  }
  
  void
  Object::do_set(Slot *p_slot, Object *p_level, InternalType *p_value)
  {
    MethodSlot *meth = dynamic_cast<MethodSlot*>(p_slot);
    if(meth == NULL)
    {
      PropertySlot *prop = dynamic_cast<PropertySlot*>(p_slot);
      if(prop->setter)
        do_call(prop->getter, p_level, p_value, NULL);
      else
        prop->value = p_value->clone();
    }
    else
    {
      printf("Can't do a set operation on a method\n");
    }
  }
  
  InternalType *
  Object::do_call(Function *func, Object *level,...)
  {
    BoundMethod m(func, this, level);
    InternalType *arg;
    va_list ap;
    typed_list args, out;
    int retcount;
    
    va_start(ap, level);
    while((arg = va_arg(ap, InternalType*)) != NULL)
      args.push_back(arg);
    va_end(ap);
    
    if(m.call(args, &retcount, out) == Function::AllGood)
      return out[0];
    else
      return NULL;
  }
  
  InternalType *
  Object::do_bind(Function *func, Object *level)
  {
    return new BoundMethod(func, this, level);
  }

  InternalType *
  ObjectMatrix::get(const std::string &p_slotName)
  {
    ObjectMatrix *sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
    
    // Handle the case of this.x with x being private
    if(sender != NULL && is_this())
    {
      if(sender->m_lvptr == m_lvptr)
      {
        InternalType * res = m_lvptr->get_priv(p_slotName, m_optr, sender);
        if(res != NULL)
          return res;
      }
    }
    
    if(is_super())
      // super.x
      return m_startptr->get(p_slotName, m_optr, sender);
    else
      // normal case
      return m_optr->get(p_slotName, m_optr, sender);
  }
  
  /**
   * ObjectMatrix
   */
  
  void
  ObjectMatrix::set(const std::string &p_slotName, InternalType * p_value)
  {
    ObjectMatrix * sender = NULL; // symbol::Context::getInstance()->get("this", true);
    
    // Handle the case of this.x = y with x being private
    if(sender != NULL && is_this())
      if(sender->m_lvptr == m_lvptr)
        if(m_lvptr->set_priv(p_slotName, p_value, m_optr, sender))
          return;
    
    if(is_super())
      // super.x = y
      m_startptr->set(p_slotName, p_value, m_optr, sender);
    else
      // normal case
      m_optr->set(p_slotName, p_value, m_optr, sender);
  }
  
  ObjectMatrix *
  ObjectMatrix::create_this(Object *p_self, Object *p_level)
  {
    ObjectMatrix *res = new ObjectMatrix;
    res->m_optr = p_self;
    res->m_lvptr = p_level;
    res->m_startptr = NULL;
    return res;
  }
  
  ObjectMatrix *
  ObjectMatrix::create_super(Object *p_self, Object *p_startp)
  {
    ObjectMatrix *res = new ObjectMatrix;
    res->m_optr = p_self;
    res->m_lvptr = NULL;
    res->m_startptr = p_startp;
    return res;
  }
  
  ObjectMatrix *
  ObjectMatrix::create_standard_ref(Object *p_self)
  {
    ObjectMatrix *res = new ObjectMatrix;
    res->m_optr = p_self;
    res->m_lvptr = NULL;
    res->m_startptr = NULL;
    return res;
  }

  /**
   * Class
   */
  Class::Class(const std::string &p_name, Object *p_isa):
    Object(p_isa),
    m_name(p_name),
    m_instance_slots()
  {
    String *s = new String(p_name.c_str());
    install_property("super", Slot::PUBLIC, ObjectMatrix::create_standard_ref(p_isa), NULL, ro_setter);
    install_property("name", Slot::PUBLIC, s, NULL, ro_setter);
  }
  
  Class *
  Class::create(const std::string &p_name, Class *p_super_class)
  {
    if(p_super_class != NULL)
      return new Class(p_name, p_super_class);
    else
      return new Class(p_name, get_root_class());
  }
  
  void
  Class::install_instance_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Function *p_getter, Function *p_setter)
  {
    if(m_instance_slots.find(p_slotName) != m_instance_slots.end())
    {
      printf("Error: slot already exists\n");
      return;
    }
    
    PropertySlot *slot = new PropertySlot;
    slot->visibility = p_visibility;
    slot->value = p_default;
    slot->getter = p_getter;
    slot->setter = p_setter;
    m_instance_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
  }
  
  void
  Class::install_instance_method(const std::string &p_slotName, Slot::Visibility p_visibility, Function *p_func)
  {
    if(m_instance_slots.find(p_slotName) != m_instance_slots.end())
    {
      printf("Error: slot already exists\n");
      return;
    }
    
    MethodSlot *slot = new MethodSlot;
    slot->visibility = p_visibility;
    slot->func = p_func;
    m_instance_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
  }
  
  Slot *
  Class::resolv_instance_slot(const std::string &p_slotName)
  {
    SlotsIterator it = m_instance_slots.find(p_slotName);
    if(it != m_instance_slots.end())
      return &it->second;
    else
      return NULL;
  }
  
  Instance *
  Class::create_instance()
  {
    Class *super_class = dynamic_cast<Class*>(m_isa);
    if(super_class)
      return new Instance(this, super_class->create_instance());
    else
      return new Instance(this, Object::get_root_object());
  }
  
  Instance  *
  Class::create_instance(typed_list &p_constructor_args)
  {
    typed_list out_args;
    int nArgsOut;
    Instance *res = create_instance();
    
    InternalType *tmp = res->get("%constructor", res, NULL);
    Function *constructor = dynamic_cast<Function*>(tmp);
    if(constructor)
      constructor->call(p_constructor_args, &nArgsOut, out_args);
    
    return res;
  }
  
  /**
   * Instance
   */
  Instance::Instance(Class *p_kls, Object *p_isa):
    m_kls(p_kls),
    Object(p_isa)
  {
    install_property("super", Slot::PRIVATE, ObjectMatrix::create_standard_ref(p_isa), NULL, ro_setter);
    install_property("klass", Slot::PUBLIC, ObjectMatrix::create_standard_ref(p_kls), NULL, ro_setter);
    install_property("level_class", Slot::PRIVATE, ObjectMatrix::create_standard_ref(p_kls), NULL, ro_setter);
  }
  
  bool
  Instance::resolv_slot_local(const std::string &p_slotName, const ObjectMatrix *p_sender,
      Slot **r_slot)
  {
    bool res = Object::resolv_slot_local(p_slotName, p_sender, r_slot);
    if(*r_slot != NULL)
      return res;
    
    Slot *slot = m_kls->resolv_instance_slot(p_slotName);
    if(slot != NULL)
    {
      if(is_visible(slot, p_sender))
        *r_slot = slot;
      return true;
    }
    
    return false;
  }
  
  /**
   * Root object
   */
  static Function::ReturnValue
  install_property(typed_list &in, int *_piRetCount, typed_list &out, ObjectMatrix *self, ObjectMatrix *super)
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
  
  static Function::ReturnValue
  install_method(typed_list &in, int *_piRetCount, typed_list &out, ObjectMatrix *self, ObjectMatrix *super)
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
  
  Object *
  Object::get_root_object()
  {
    static Object *obj = NULL;
    if(obj == NULL)
    {
      obj = new Object();
      obj->install_method("%install_property", Slot::PUBLIC, new Method(&types::install_property));
      obj->install_method("%install_method", Slot::PUBLIC, new Method(&types::install_method));
      // TODO: remove_slot, slots_list, %get, %set
    }
    return obj;
  }
  
  /**
   * Root class
   */
  static Function::ReturnValue
  new_instance(typed_list &in, int *_piRetCount, typed_list &out, ObjectMatrix *self, ObjectMatrix *super)
  {
    Class *kls = dynamic_cast<Class*>(self->object_ref_get());
    Instance *inst = kls->create_instance(in);
    *_piRetCount = 1;
    out.push_back(ObjectMatrix::create_standard_ref(inst));
    return Function::AllGood;
  }
  
  static Function::ReturnValue
  empty_constructor(typed_list &in, int *_piRetCount, typed_list &out, ObjectMatrix *self, ObjectMatrix *super)
  {
    *_piRetCount = 0;
    return Function::AllGood;
  }
  
  Class *
  Class::get_root_class()
  {
    static Class *kls = NULL;
    if(kls == NULL)
    {
      kls = new Class("<RootClass>", get_root_object());
      kls->install_method("%new", Slot::PUBLIC, new Method(&types::new_instance));
      kls->install_instance_method("%constructor", Slot::PUBLIC, new Method(&types::empty_constructor));
      // TODO: install_instance_method, install_instance_property, instance_slots_list, remove_instance_slot
    }
    return kls;
  }
  
  static Function::ReturnValue
  ro_setter_func(typed_list &in, int *_piRetCount, typed_list &out)
  {
    printf("Read only property");
    return Function::AllGood;
  }
  
  Function *ro_setter = new Function("", ro_setter_func, "");

  /**
   * Other
   */
  Function::ReturnValue
  Method::call(typed_list &in, int* _piRetCount, typed_list &out)
  {
    symbol::Context *ctx = symbol::Context::getInstance();
    ObjectMatrix *thisref = dynamic_cast<ObjectMatrix*>(ctx->get(symbol::Symbol("this")));
    ObjectMatrix *superref = dynamic_cast<ObjectMatrix*>(ctx->get(symbol::Symbol("super")));
    return m_callback(in, _piRetCount, out, thisref, superref);
  }
}
