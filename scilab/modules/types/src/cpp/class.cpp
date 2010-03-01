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

#include "class.hxx"
#include "objectmatrix.hxx"
#include "ooutils.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "instance.hxx"
#include <sstream>

namespace types
{
	/*
	 * Root class methods declaration
	 */
	
	static Callable::ReturnValue RootNew(typed_list &, int, typed_list &, const Method::MethodCallCtx &);
	static Callable::ReturnValue EmptyConstructor(typed_list &, int, typed_list &, const Method::MethodCallCtx &);
	static Callable::ReturnValue InstallInstanceMethod(typed_list &, int, typed_list &, const Method::MethodCallCtx &);
	static Callable::ReturnValue InstallInstanceProperty(typed_list &, int, typed_list &, const Method::MethodCallCtx &);
	static Callable::ReturnValue RemoveInstanceSlot(typed_list &, int, typed_list &, const Method::MethodCallCtx &);
	static Callable::ReturnValue InstanceSlotsList(typed_list &, int, typed_list &, const Method::MethodCallCtx &);
	
	/*
	 * Class::*
	 */
	
	Class::Class(const std::string &p_name, Object *p_isa):
		Object(p_isa),
		m_instanceSlots(),
		m_name(p_name)
	{
		String *s = new String(p_name.c_str());
		InstallProperty("super", Slot::PUBLIC, ObjectMatrix::CreateStandardRef(p_isa), NULL, RoSetter);
		InstallProperty("name", Slot::PUBLIC, s, NULL, RoSetter);
	}
	
	Class::~Class()
	{
		std::map<std::string, Slot&>::iterator it;
		for(it = m_instanceSlots.begin(); it != m_instanceSlots.end(); ++it)
		{
			delete &it->second;
		}
	}
	
	Class* Class::Create(const std::string &p_name, Class *p_super_class)
	{
		if(p_super_class != NULL)
		{
			return new Class(p_name, p_super_class);
		}
		else
		{
			return new Class(p_name, GetRootClass());
		}
	}
	
	void Class::InstallInstanceProperty(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *p_getter, Callable *p_setter)
	{
		if(m_instanceSlots.find(p_slotName) != m_instanceSlots.end())
		{
		  std::cerr <<"Error: slot already exists" << std::endl;
			return;
		}
		
		/* "Default default" is [] */
		if(p_default == NULL)
		{
			p_default = new Double(0,0);
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
		m_instanceSlots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
	}
	
	void Class::InstallInstanceMethod(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func)
	{
		if(m_instanceSlots.find(p_slotName) != m_instanceSlots.end())
		{
		  std::cerr << "Error: slot already exists" << std::endl;
			return;
		}
		
		/* Create slot */
		MethodSlot *slot = new MethodSlot;
		slot->name = p_slotName;
		slot->visibility = p_visibility;
		slot->func = p_func;
		
		/* Keep a ref on the code */
		slot->func->IncreaseRef();
		
		/* Add slot */
		m_instanceSlots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
	}
	
	Slot* Class::ResolvInstanceSlot(const std::string &p_slotName)
	{
		SlotsIterator it = m_instanceSlots.find(p_slotName);
		if(it != m_instanceSlots.end())
		{
			return &it->second;
		}
		else
		{
			return NULL;
		}
	}
	
	Instance* Class::CreateInstance()
	{
		Class *super_class = dynamic_cast<Class*>(m_pSuper);
		if(super_class)
		{
			return new Instance(this, super_class->CreateInstance());
		}
		else
		{
			return new Instance(this, Object::GetRootObject());
		}
	}
	
	Instance* Class::CreateInstance(typed_list &p_constructor_args)
	{
		typed_list out_args;
		Instance *res = CreateInstance();
		
		InternalType *tmp = res->Get("%constructor", res, NULL);
		Callable *constructor = dynamic_cast<Callable*>(tmp);
		if(constructor)
		{
			constructor->call(p_constructor_args, 0, out_args);
		}
		
		return res;
	}
	
	Class* Class::GetRootClass()
	{
		static Class *kls = NULL;
		if(kls == NULL)
		{
			kls = new Class("<RootClass>", GetRootObject());
			kls->InstallMethod("%new", Slot::PUBLIC, new Method(&types::RootNew));
			kls->InstallMethod("%install_instance_method", Slot::PUBLIC, new Method(&types::InstallInstanceMethod));
			kls->InstallMethod("%install_instance_property", Slot::PUBLIC, new Method(&types::InstallInstanceProperty));
			kls->InstallMethod("%instance_slots_list", Slot::PUBLIC, new Method(&types::InstanceSlotsList));
			kls->InstallMethod("%remove_instance_slot", Slot::PUBLIC, new Method(&types::RemoveInstanceSlot));
			kls->InstallInstanceMethod("%constructor", Slot::PUBLIC, new Method(&types::EmptyConstructor));
		}
		return kls;
	}
	
	void Class::RemoveInstanceSlot(const std::string &_slotName)
	{
		if(m_instanceSlots.find(_slotName) != m_instanceSlots.end())
		{
			Slot *theSlot = &m_instanceSlots.find(_slotName)->second;
			m_instanceSlots.erase(m_instanceSlots.find(_slotName));
			delete theSlot;
		}
		else
		{
			if(m_pSuper && dynamic_cast<Class*>(m_pSuper))
			{
				dynamic_cast<Class*>(m_pSuper)->RemoveInstanceSlot(_slotName);
			}
			else
			{
				std::stringstream ss;
				ss << "Error: slot " <<	_slotName << " not found";
				throw ss.str();
			}
		}
	}
	
	/*
	 * Root class methods definition
	 */
	
	static Callable::ReturnValue RootNew(typed_list &in, int, typed_list &out, const Method::MethodCallCtx &ctx)
	{	/* Accepts any input argument since they are transmitted to the constructor */
		Class *kls = dynamic_cast<Class*>(ctx.self.GetObjectRef());
		Instance *inst = kls->CreateInstance(in);
		out.push_back(ObjectMatrix::CreateStandardRef(inst));
		return Callable::OK;
	}
	
	static Callable::ReturnValue EmptyConstructor(typed_list &in, int, typed_list &, const Method::MethodCallCtx &)
	{
		if(in.size() != 1)
		{
			return Callable::Error;
		}
		else
		{
			return Callable::OK_NoResult;
		}
	}

	static Callable::ReturnValue RemoveInstanceSlot(typed_list &in, int retCount, typed_list &out, const Method::MethodCallCtx &ctx)
	{
		if(in.size() != 1)
		{
			return Callable::Error;
		}
		
		String *name = dynamic_cast<String*>(in[0]);
		Class *kls = dynamic_cast<Class*>(ctx.self.GetObjectRef());
		
		if(name == NULL || kls == NULL)
		{
			return Callable::Error;
		}
		
		kls->RemoveInstanceSlot(name->string_get(0,0));
		
		return Callable::OK_NoResult;
	}
	
	static Callable::ReturnValue InstallInstanceProperty(typed_list &in, int retCount, typed_list &, const Method::MethodCallCtx &ctx)
	{
		if(in.size() == 0 || in.size() > 5)
		{
			return Callable::Error;
		}
		
		String *name = dynamic_cast<String*>(in[0]);
		Class *kls = dynamic_cast<Class*>(ctx.self.GetObjectRef());
		InternalType *def = NULL;
		Callable *getter = NULL;
		Callable *setter = NULL;
		Slot::Visibility svisib = Slot::PUBLIC;
		
		/* name */
		if(name == NULL)
		{
			return Callable::Error;
		}
		
		/* visibility */
		if(in.size() > 1)
		{
			String *visibility = dynamic_cast<String*>(in[1]);
			if(visibility == NULL)
			{
				return Callable::Error;
			}
			else
			{
				Slot::Visibility svisib = Slot::PUBLIC;
				if(visibility != NULL)
				{
					if(!strcmp(visibility->string_get(0,0), "public"))
						svisib = Slot::PUBLIC;
					else if(!strcmp(visibility->string_get(0,0), "protected"))
						svisib = Slot::PROTECTED;
					else if(!strcmp(visibility->string_get(0,0), "private"))
						svisib = Slot::PRIVATE;
					else
						throw std::string("Bad visibility");
				}
			}
		}
		
		/* default */
		if(in.size() > 2)
		{
			def = in[2];
		}
		
		/* getter */
		if(in.size() > 3)
		{
			getter = dynamic_cast<Callable*>(in[3]);
			if(getter == NULL)
			{
				return Callable::Error;
			}
		}
		
		/* setter */
		if(in.size() > 4)
		{
			setter = dynamic_cast<Callable*>(in[4]);
			if(setter == NULL)
			{
				return Callable::Error;
			}
		}
		
		kls->InstallInstanceProperty(name->string_get(0,0), svisib, def, getter, setter);
		
		return Callable::OK_NoResult;
	}
	
	static Callable::ReturnValue InstallInstanceMethod(typed_list &in, int retCount, typed_list &, const Method::MethodCallCtx &ctx)
	{
		if(in.size() != 3)
		{
			return Callable::Error;
		}
		
		String *name = dynamic_cast<String*>(in[0]);
		String *visibility = dynamic_cast<String*>(in[1]);
		Class *kls = dynamic_cast<Class*>(ctx.self.GetObjectRef());
		Callable *func = dynamic_cast<Callable*>(in[2]);
		
		if(name == NULL || visibility == NULL || func == NULL)
		{
			return Callable::Error;
		}
		
		Slot::Visibility svisib = Slot::PUBLIC;
		if(visibility != NULL)
		{
			if(!strcmp(visibility->string_get(0,0), "public"))
				svisib = Slot::PUBLIC;
			else if(!strcmp(visibility->string_get(0,0), "protected"))
				svisib = Slot::PROTECTED;
			else if(!strcmp(visibility->string_get(0,0), "private"))
				svisib = Slot::PRIVATE;
			else
				throw std::string("Bad visibility");
		}
		
		kls->InstallInstanceMethod(name->string_get(0,0), svisib, func);
		
		return Callable::OK_NoResult;
	}

	static Callable::ReturnValue InstanceSlotsList(typed_list &in, int retCount, typed_list &out, const Method::MethodCallCtx &ctx)
	{
		if(in.size() > 1)
		{
			return Callable::Error;
		}
		
		if(retCount == 0 || retCount > 3)
		{
			return Callable::Error;
		}
		
		/* Recursive ? */
		bool recursive = true;
		
		if(in.size() > 0)
		{
			Bool *sciRec = in[0]->getAsBool();
			if(sciRec == NULL)
			{
				return Callable::Error;
			}
			if(sciRec->size_get() != 1)
			{
				return Callable::Error;
			}
			recursive = sciRec->bool_get(0, 0);
		}
		
		/* Get slots */
		std::map<std::string, Slot&> allSlots;
		std::map<std::string, Slot&>::const_iterator it;
		Class *lvl = dynamic_cast<Class*>(ctx.super.GetObjectRef());
		
		do
		{
			for(it = lvl->GetInstanceSlots().begin() ; it != lvl->GetInstanceSlots().end() ; ++it)
			{
				if(allSlots.find(it->first) == allSlots.end())
				{
					allSlots.insert(std::pair<std::string, Slot&>(it->first, it->second));
				}
				else
				{
					if(allSlots.find(it->first)->second.visibility == Slot::PRIVATE || it->second.visibility == Slot::PUBLIC)
					{ /* New slots is more visibility that the old one */
						allSlots.insert(std::pair<std::string, Slot&>(it->first, it->second));
					}
				}
			}
			lvl = dynamic_cast<Class*>(lvl->Super());
		}
		while(lvl != NULL && recursive);
		
		/* Build return */
		int row;
		
		/* Name */
		String *retName = new String(allSlots.size(), 1);
		for(row = 0, it = allSlots.begin() ; it != allSlots.end() ; ++it, ++row)
		{
			retName->string_set(row, 0, it->first.c_str());
		}
		out.push_back(retName);
		
		/* Type */
		if(retCount > 1)
		{
			String *retType = new String(allSlots.size(), 1);
			for(row = 0, it = allSlots.begin() ; it != allSlots.end() ; ++it, ++row)
			{
				if(dynamic_cast<PropertySlot*>(&it->second))
				{
					retType->string_set(row, 0, "property");
				}
				else
				{
					retType->string_set(row, 0, "method");
				}
			}
			out.push_back(retType);
		}
		
		/* Visibility */
		if(retCount > 2)
		{
			String *retVisibility = new String(allSlots.size(), 1);
			for(row = 0, it = allSlots.begin() ; it != allSlots.end() ; ++it, ++row)
			{
				switch(it->second.visibility)
				{
					case Slot::PUBLIC:
						retVisibility->string_set(row, 0, "public");
						break;
					case Slot::PROTECTED:
						retVisibility->string_set(row, 0, "protected");
						break;
					case Slot::PRIVATE:
						retVisibility->string_set(row, 0, "private");
						break;
					default:;
				}
			}
			out.push_back(retVisibility);
		}
		
		return Callable::OK;
	}
}
