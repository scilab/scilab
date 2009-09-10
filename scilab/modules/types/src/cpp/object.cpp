/*
*	Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*	Copyright (C) 2009-2009 - DIGITEO - Simon LIPP
*
*	This file must be used under the terms of the CeCILL.
*	This source file is licensed as described in the file COPYING, which
*	you should have received as part of this distribution.	The terms
*	are also available at
*	http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <cstdarg>

#include "object.hxx"
#include "ooutils.hxx"
#include "ooutils-private.hxx"
#include "context.hxx"

namespace types
{
	PropertySlot::~PropertySlot()
	{
		if(getter)
		{
			getter->DecreaseRef();
			if(getter->isDeletable())
			{
				delete getter;
			}
		}
		
		if(setter)
		{
			setter->DecreaseRef();
			if(setter->isDeletable())
			{
				delete setter;
			}
		}
		
		default_value->DecreaseRef();
		if(default_value->isDeletable())
		{
			delete default_value;
		}
	}
	
	MethodSlot::~MethodSlot()
	{
		func->DecreaseRef();
		if(func->isDeletable())
		{
			delete func;
		}
	}
	
	Object::~Object()
	{
		std::map<std::string, InternalType *>::iterator it;
		for(it = m_slots_values.begin(); it != m_slots_values.end(); ++it)
		{
			it->second->DecreaseRef();
			if(it->second->isDeletable())
			{
				delete it->second;
			}
		}
	}
	
	void Object::install_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *p_getter, Callable *p_setter)
	{
		SlotsIterator it = m_slots.find(p_slotName);
		if(it != m_slots.end())
		{
			throw std::string("Error: slot already exists");
		}
		
		if(p_default == NULL)
		{
			p_default = new Double(0, 0);
		}
		
		/* Create slot */
		PropertySlot *slot = new PropertySlot;
		slot->name = p_slotName;
		slot->visibility = p_visibility;
		slot->default_value = p_default;
		slot->getter = p_getter;
		slot->setter = p_setter;
		
		/* Keep a ref on getter, setter and default */
		slot->default_value->IncreaseRef();
		if(slot->getter)
		{
			slot->getter->IncreaseRef();
		}
		if(slot->setter)
		{
			slot->setter->IncreaseRef();
		}
		
		/* Add slot */
		m_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
	}
	
	void
	Object::install_method(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func)
	{
		SlotsIterator it = m_slots.find(p_slotName);
		if(it != m_slots.end())
		{
			throw std::string("Error: slot already exists");
		}
		
		/* Create slot */
		MethodSlot *slot = new MethodSlot;
		slot->name = p_slotName;
		slot->visibility = p_visibility;
		slot->func = p_func;
		
		/* Keep a ref on the code */
		slot->func->IncreaseRef();
		
		/* Add slot */
		m_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
	}
	
	bool Object::resolv_slot(const std::string &p_slotName, const ObjectMatrix *p_sender, Object **r_lvptr, Slot **r_slot)
	{
		bool res = resolv_slot_local(p_slotName, p_sender, r_slot);
		if(*r_slot != NULL)
		{	/* Found at this level */
			*r_lvptr = this;
			return true;
		}
		else
		{	/* Try upper level, or fail */
			if(m_isa != NULL)
			{
				res |= m_isa->resolv_slot(p_slotName, p_sender, r_lvptr, r_slot);
			}
			else
			{
				*r_lvptr = NULL;
			}
			return res;
		}
	}

	bool Object::resolv_slot_local(const std::string &p_slotName, const ObjectMatrix *p_sender, Slot **r_slot)
	{
		SlotsIterator it = m_slots.find(p_slotName);
		*r_slot = NULL;
		if(it != m_slots.end())
		{
			if(is_visible(&it->second, p_sender))
			{
				*r_slot = &it->second;
			}
			return true;
		}
		return false;
	}

	bool Object::is_visible(const Slot *p_slot, const ObjectMatrix *p_sender) const
	{
		// Public: always visible
		if(p_slot->visibility == Slot::PUBLIC)
		{
			return true;
		}
		
		// Protected and private can’t be visible from a function
		if(p_sender == NULL)
		{
			return false;
		}
		
		// Private: visible if sender level is current object
		if(p_slot->visibility == Slot::PRIVATE)
		{
			return p_sender->level_ref_get() == this;
		}
		
		// Protected: visible if current object is in the sender super chain
		const Object *cur_level = p_sender->object_ref_get();
		while(cur_level)
		{
			if(cur_level == this)
			{
				return true;
			}
			cur_level = cur_level->m_isa;
		}
		return false;
	}
	
	InternalType* Object::get(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		Object *level;
		
		// Standard slots
		bool res = resolv_slot(p_slotName, p_sender, &level, &slot);
		if(slot != NULL && level != NULL)
		{
			return p_self->do_get(slot, level, p_sender);
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
			{
				BoundMethod m(magic->func, p_self, level, p_sender);
				return p_self->do_call(m, 1, new String(p_slotName.c_str()), NULL);
			}
		}
		
		// No magic method, error
		{
			std::stringstream ss;
			ss << "Error: slot " <<	p_slotName << " not found";
			throw ss.str();
		}
	}
	
	InternalType* Object::get_priv(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		resolv_slot_local(p_slotName, p_sender, &slot);
		if(slot != NULL)
		{
			if(slot->visibility == Slot::PRIVATE)
			{
				return p_self->do_get(slot, this, p_sender);
			}
		}
		return NULL;
	}

	void Object::set(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		Object *level;
		
		// Standard slots
		bool res = resolv_slot(p_slotName, p_sender, &level, &slot);
		if(slot != NULL && level != NULL)
		{
			p_self->do_set(slot, level, p_value, p_sender);
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
				BoundMethod m(magic->func, p_self, level, p_sender);
				p_self->do_call(m, 1, new String(p_slotName.c_str()), p_value, NULL);
				return;
			}
		}
		
		// No magic method, error
		{
			std::stringstream ss;
			ss << "Error: slot " <<	p_slotName << " not found";
			throw ss.str();
		}
	}
	
	bool Object::set_priv(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		resolv_slot_local(p_slotName, p_sender, &slot);
		if(slot != NULL)
		{
			if(slot->visibility == Slot::PRIVATE)
			{
				p_self->do_set(slot, this, p_value, p_sender);
				return true;
			}
		}
		return false;
	}
	
	InternalType* Object::do_get(Slot *p_slot, Object *p_level, ObjectMatrix *p_sender)
	{
		MethodSlot *meth = dynamic_cast<MethodSlot*>(p_slot);
		if(meth == NULL)
		{
			PropertySlot *prop = dynamic_cast<PropertySlot*>(p_slot);
			if(prop->getter)
			{
				BoundGetter m(prop, this, p_level, p_sender);
				return do_call(m, 1, NULL);
			}
			else
			{
				return p_level->raw_get(*prop);
			}
		}
		else
		{
			return new BoundMethod(meth->func, this, p_level, p_sender);
		}
	}
	
	void Object::do_set(Slot *p_slot, Object *p_level, InternalType *p_value, ObjectMatrix *p_sender)
	{
		MethodSlot *meth = dynamic_cast<MethodSlot*>(p_slot);
		if(meth == NULL)
		{
			PropertySlot *prop = dynamic_cast<PropertySlot*>(p_slot);
			if(prop->setter)
			{
				BoundSetter m(prop, this, p_level, p_sender);
				do_call(m, 1, p_value, NULL);
			}
			else
			{
				p_level->raw_set(*prop, p_value);
			}
		}
		else
		{
			throw std::string("Can't do a set operation on a method");
		}
	}
	
	InternalType* Object::do_call(Callable &m, int retCount, ...)
	{
		InternalType *arg;
		va_list ap;
		typed_list args, out;
		
		va_start(ap, retCount);
		while((arg = va_arg(ap, InternalType*)) != NULL)
		{
			args.push_back(arg);
		}
		va_end(ap);
		
		if(m.call(args, retCount, out) == Callable::OK && out.size() != 0)
		{
			return out[0];
		}
		
		return NULL;
	}
	
	Object* Object::get_root_object()
	{
		static Object *obj = NULL;
		if(obj == NULL)
		{
			obj = new Object();
			obj->install_method("%install_property", Slot::PUBLIC, new Method(&types::install_property));
			obj->install_method("%install_method", Slot::PUBLIC, new Method(&types::install_method));
			obj->install_method("%get", Slot::PUBLIC, new Method(&types::object_get));
			obj->install_method("%set", Slot::PUBLIC, new Method(&types::object_set));
			obj->install_method("%slots_list", Slot::PUBLIC, new Method(&types::slots_list));
			obj->install_method("%remove_slot", Slot::PUBLIC, new Method(&types::remove_slot));
		}
		return obj;
	}
	
	InternalType* Object::raw_get(PropertySlot &slot)
	{
		std::map<std::string, InternalType *>::iterator it = m_slots_values.find(slot.name);
		if(it == m_slots_values.end())
		{
			InternalType *ret = slot.default_value->clone();
			m_slots_values[slot.name] = ret;
			ret->IncreaseRef();
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
		{
			it->second->DecreaseRef();
			if(it->second->isDeletable())
			{
				delete it->second;
			}
		}
		
		m_slots_values[slot.name] = value;
		value->IncreaseRef();
	}
	
	void Object::RemoveSlot(const std::string &_slotName)
	{
		if(m_slots.find(_slotName) != m_slots.end())
		{
			m_slots.erase(m_slots.find(_slotName));
		}
		else
		{
			if(m_isa)
			{
				m_isa->RemoveSlot(_slotName);
			}
			else
			{
				std::stringstream ss;
				ss << "Error: slot " <<	_slotName << " not found";
				throw ss.str();
			}
		}
	}
}
