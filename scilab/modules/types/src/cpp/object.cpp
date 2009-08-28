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
#include "ooutils.hxx"
#include "ooutils-private.hxx"
#include "context.hxx"

namespace types
{
  void
  Object::install_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Function *p_getter, Function *p_setter)
  {
    SlotsIterator it = m_slots.find(p_slotName);
    if(it != m_slots.end())
      throw std::string("Error: slot already exists");
    
    PropertySlot *slot = new PropertySlot;
    slot->name = p_slotName;
    slot->visibility = p_visibility;
    slot->default_value = p_default;
    slot->getter = p_getter;
    slot->setter = p_setter;
    m_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
  }
  
  void
  Object::install_method(const std::string &p_slotName, Slot::Visibility p_visibility, Function *p_func)
  {
    SlotsIterator it = m_slots.find(p_slotName);
    if(it != m_slots.end())
      throw std::string("Error: slot already exists");
    
    MethodSlot *slot = new MethodSlot;
    slot->name = p_slotName;
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
      std::stringstream ss;
      ss << "Error: slot " << p_slotName << " found but not visible from this context";
      throw ss.str();
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
    {
      std::stringstream ss;
      ss << "Error: slot " <<  p_slotName << " not found";
      throw ss.str();
    }
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
      std::stringstream ss;
      ss << "Error: slot " << p_slotName << " found but not visible from this context";
      throw ss.str();
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
    {
      std::stringstream ss;
      ss << "Error: slot " <<  p_slotName << " not found";
      throw ss.str();
    }
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
        return raw_get(*prop);
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
        do_call(prop->setter, p_level, p_value, NULL);
      else
        raw_set(*prop, p_value);
    }
    else
    {
      throw std::string("Can't do a set operation on a method");
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
    
    if(m.call(args, &retcount, out) == Function::OK)
      return out[0];
    else
      return NULL;
  }
  
  InternalType *
  Object::do_bind(Function *func, Object *level)
  {
    return new BoundMethod(func, this, level);
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
  
  InternalType *
  Object::raw_get(PropertySlot &slot)
  {
    std::map<std::string, InternalType *>::iterator it = m_slots_values.find(slot.name);
    if(it == m_slots_values.end())
    {
      InternalType *ret = slot.default_value->clone();
      m_slots_values[slot.name] = ret;
      ret->DenyDelete();
      return ret;
    }
    else
    {
      return it->second;
    }
  }
  
  void Object::raw_set(PropertySlot &slot, InternalType *value)
  {
    std::map<std::string, InternalType *>::iterator it = m_slots_values.find(slot.name);
    if(it != m_slots_values.end())
      delete it->second;
    
    m_slots_values[slot.name] = value;
    value->DenyDelete();
  }
}
