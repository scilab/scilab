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
		std::map<std::string, InternalType*>::iterator it;
		for(it = m_slotsValues.begin(); it != m_slotsValues.end(); ++it)
		{
			it->second->DecreaseRef();
			if(it->second->isDeletable())
			{
				delete it->second;
			}
		}
	}
	
	void Object::InstallProperty(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *p_getter, Callable *p_setter)
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
	Object::InstallMethod(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func)
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
	
	bool Object::ResolvSlot(const std::string &p_slotName, const ObjectMatrix *p_sender, Object **r_lvptr, Slot **r_slot)
	{
		bool res = ResolvSlotLocal(p_slotName, p_sender, r_slot);
		if(*r_slot != NULL)
		{	/* Found at this level */
			*r_lvptr = this;
			return true;
		}
		else
		{	/* Try upper level, or fail */
			if(m_pSuper != NULL)
			{
				res |= m_pSuper->ResolvSlot(p_slotName, p_sender, r_lvptr, r_slot);
			}
			else
			{
				*r_lvptr = NULL;
			}
			return res;
		}
	}

	bool Object::ResolvSlotLocal(const std::string &p_slotName, const ObjectMatrix *p_sender, Slot **r_slot)
	{
		SlotsIterator it = m_slots.find(p_slotName);
		*r_slot = NULL;
		if(it != m_slots.end())
		{
			if(IsVisible(&it->second, p_sender))
			{
				*r_slot = &it->second;
			}
			return true;
		}
		return false;
	}

	bool Object::IsVisible(const Slot *p_slot, const ObjectMatrix *p_sender) const
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
			return p_sender->GetLevelRef() == this;
		}
		
		// Protected: visible if current object is in the sender super chain
		const Object *cur_level = p_sender->GetObjectRef();
		while(cur_level)
		{
			if(cur_level == this)
			{
				return true;
			}
			cur_level = cur_level->m_pSuper;
		}
		return false;
	}
	
	InternalType* Object::Get(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		Object *level;
		
		// Standard slots
		bool res = ResolvSlot(p_slotName, p_sender, &level, &slot);
		if(slot != NULL && level != NULL)
		{
			return p_self->DoGet(slot, level, p_sender);
		}
		else if(res)
		{
			std::stringstream ss;
			ss << "Error: slot " << p_slotName << " found but not visible from this context";
			throw ss.str();
		}
		
		// Not found: try to call a magic method
		ResolvSlot("%get_slot", NULL, &level, &slot);
		if(slot != NULL && level != NULL)
		{
			MethodSlot *magic = dynamic_cast<MethodSlot*>(slot);
			if(magic)
			{
				InternalType *pRet;
				{
					BoundMethod m(magic->func, p_self, level, p_sender);
					pRet = p_self->DoCall(m, 1, new String(p_slotName.c_str()), NULL);
					pRet->IncreaseRef();
				}
				pRet->DecreaseRef();
				return pRet;
			}
		}
		
		// No magic method, error
		{
			std::stringstream ss;
			ss << "Error: slot " <<	p_slotName << " not found";
			throw ss.str();
		}
	}
	
	InternalType* Object::GetPriv(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		ResolvSlotLocal(p_slotName, p_sender, &slot);
		if(slot != NULL)
		{
			if(slot->visibility == Slot::PRIVATE)
			{
				return p_self->DoGet(slot, this, p_sender);
			}
		}
		return NULL;
	}

	void Object::Set(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		Object *level;
		
		// Standard slots
		bool res = ResolvSlot(p_slotName, p_sender, &level, &slot);
		if(slot != NULL && level != NULL)
		{
			p_self->DoSet(slot, level, p_value, p_sender);
			return;
		}
		else if(res)
		{
			std::stringstream ss;
			ss << "Error: slot " << p_slotName << " found but not visible from this context";
			throw ss.str();
		}
		
		// Not found: try to call a magic method
		ResolvSlot("%set_slot", NULL, &level, &slot);
		if(slot != NULL && level != NULL)
		{
			MethodSlot *magic = dynamic_cast<MethodSlot*>(slot);
			if(magic)
			{
				BoundMethod m(magic->func, p_self, level, p_sender);
				p_value->IncreaseRef();
				p_self->DoCall(m, 0, new String(p_slotName.c_str()), p_value, NULL);
				p_value->DecreaseRef();
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
	
	bool Object::SetPriv(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender)
	{
		Slot *slot;
		ResolvSlotLocal(p_slotName, p_sender, &slot);
		if(slot != NULL)
		{
			if(slot->visibility == Slot::PRIVATE)
			{
				p_self->DoSet(slot, this, p_value, p_sender);
				return true;
			}
		}
		return false;
	}
	
	InternalType* Object::DoGet(Slot *p_slot, Object *p_level, ObjectMatrix *p_sender)
	{
		MethodSlot *meth = dynamic_cast<MethodSlot*>(p_slot);
		if(meth == NULL)
		{
			PropertySlot *prop = dynamic_cast<PropertySlot*>(p_slot);
			if(prop->getter)
			{
				InternalType *pRet;
				{
					BoundGetter m(prop, this, p_level, p_sender);
					pRet = DoCall(m, 1, NULL);
					pRet->IncreaseRef();
				}
				pRet->DecreaseRef();
				return pRet;
			}
			else
			{
				return p_level->RawGet(*prop);
			}
		}
		else
		{
			return new BoundMethod(meth->func, this, p_level, p_sender);
		}
	}
	
	void Object::DoSet(Slot *p_slot, Object *p_level, InternalType *p_value, ObjectMatrix *p_sender)
	{
		MethodSlot *meth = dynamic_cast<MethodSlot*>(p_slot);
		if(meth == NULL)
		{
			PropertySlot *prop = dynamic_cast<PropertySlot*>(p_slot);
			if(prop->setter)
			{
				p_value->IncreaseRef();
				{
					BoundSetter m(prop, this, p_level, p_sender);
					DoCall(m, 1, p_value, NULL);
				}
				p_value->DecreaseRef();
			}
			else
			{
				p_level->RawSet(*prop, p_value);
			}
		}
		else
		{
			throw std::string("Can't do a set operation on a method");
		}
	}
	
	InternalType* Object::DoCall(Callable &m, int retCount, ...)
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
	
	Object* Object::GetRootObject()
	{
		static Object *obj = NULL;
		if(obj == NULL)
		{
			obj = new Object();
			obj->InstallMethod("%install_property", Slot::PUBLIC, new Method(&types::InstallProperty));
			obj->InstallMethod("%install_method", Slot::PUBLIC, new Method(&types::InstallMethod));
			obj->InstallMethod("%get", Slot::PUBLIC, new Method(&types::ObjectGet));
			obj->InstallMethod("%set", Slot::PUBLIC, new Method(&types::ObjectSet));
			obj->InstallMethod("%slots_list", Slot::PUBLIC, new Method(&types::SlotsList));
			obj->InstallMethod("%remove_slot", Slot::PUBLIC, new Method(&types::RemoveSlot));
		}
		return obj;
	}
	
	InternalType* Object::RawGet(PropertySlot &slot)
	{
		std::map<std::string, InternalType *>::iterator it = m_slotsValues.find(slot.name);
		if(it == m_slotsValues.end())
		{
			InternalType *ret = slot.default_value;
			m_slotsValues[slot.name] = ret;
			ret->IncreaseRef();
			return ret;
		}
		else
		{
			return it->second;
		}
	}
	
	void Object::RawSet(PropertySlot &slot, InternalType *value)
	{
		std::map<std::string, InternalType *>::iterator it = m_slotsValues.find(slot.name);
		if(it != m_slotsValues.end())
		{
			it->second->DecreaseRef();
			if(it->second->isDeletable())
			{
				delete it->second;
			}
		}
		
		m_slotsValues[slot.name] = value;
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
			if(m_pSuper)
			{
				m_pSuper->RemoveSlot(_slotName);
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
