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

#include "class.hxx"
#include "objectmatrix.hxx"
#include "ooutils.hxx"
#include "string.hxx"
#include "instance.hxx"

namespace types
{
  Class::Class(const std::string &p_name, Object *p_isa):
    Object(p_isa),
    m_instance_slots(),
    m_name(p_name)
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
  Class::install_instance_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *p_getter, Callable *p_setter)
  {
    if(m_instance_slots.find(p_slotName) != m_instance_slots.end())
    {
      printf("Error: slot already exists\n");
      return;
    }
    
    PropertySlot *slot = new PropertySlot;
    slot->name = p_slotName;
    slot->visibility = p_visibility;
    slot->default_value = p_default;
    slot->getter = p_getter;
    slot->setter = p_setter;
    m_instance_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
  }
  
  void
  Class::install_instance_method(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func)
  {
    if(m_instance_slots.find(p_slotName) != m_instance_slots.end())
    {
      printf("Error: slot already exists\n");
      return;
    }
    
    MethodSlot *slot = new MethodSlot;
    slot->name = p_slotName;
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
    int nArgsOut = 0;
    Instance *res = create_instance();
    
    InternalType *tmp = res->get("%constructor", res, NULL);
    Callable *constructor = dynamic_cast<Callable*>(tmp);
    if(constructor)
      constructor->call(p_constructor_args, nArgsOut, out_args);
    
    return res;
  }
  
  /* Root class */
  
  static Callable::ReturnValue
  new_instance(typed_list &in, int, typed_list &out, ObjectMatrix *self, ObjectMatrix *)
  {
    Class *kls = dynamic_cast<Class*>(self->object_ref_get());
    Instance *inst = kls->create_instance(in);
    out.push_back(ObjectMatrix::create_standard_ref(inst));
    return Callable::OK;
  }
  
  static Callable::ReturnValue
  empty_constructor(typed_list &, int, typed_list &, ObjectMatrix *, ObjectMatrix *)
  {
    return Callable::OK;
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
}
