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
		m_instance_slots(),
		m_name(p_name)
	{
		String *s = new String(p_name.c_str());
		install_property("super", Slot::PUBLIC, ObjectMatrix::create_standard_ref(p_isa), NULL, ro_setter);
		install_property("name", Slot::PUBLIC, s, NULL, ro_setter);
	}
	
	Class* Class::create(const std::string &p_name, Class *p_super_class)
	{
		if(p_super_class != NULL)
		{
			return new Class(p_name, p_super_class);
		}
		else
		{
			return new Class(p_name, get_root_class());
		}
	}
	
	void Class::install_instance_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *p_getter, Callable *p_setter)
	{
		if(m_instance_slots.find(p_slotName) != m_instance_slots.end())
		{
			printf("Error: slot already exists\n");
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
		m_instance_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
	}
	
	void Class::install_instance_method(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func)
	{
		if(m_instance_slots.find(p_slotName) != m_instance_slots.end())
		{
			printf("Error: slot already exists\n");
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
		m_instance_slots.insert(std::pair<const std::string &, Slot&>(p_slotName, *slot));
	}
	
	Slot* Class::resolv_instance_slot(const std::string &p_slotName)
	{
		SlotsIterator it = m_instance_slots.find(p_slotName);
		if(it != m_instance_slots.end())
		{
			return &it->second;
		}
		else
		{
			return NULL;
		}
	}
	
	Instance* Class::create_instance()
	{
		Class *super_class = dynamic_cast<Class*>(m_isa);
		if(super_class)
		{
			return new Instance(this, super_class->create_instance());
		}
		else
		{
			return new Instance(this, Object::get_root_object());
		}
	}
	
	Instance* Class::create_instance(typed_list &p_constructor_args)
	{
		typed_list out_args;
		Instance *res = create_instance();
		
		InternalType *tmp = res->get("%constructor", res, NULL);
		Callable *constructor = dynamic_cast<Callable*>(tmp);
		if(constructor)
		{
			constructor->call(p_constructor_args, 0, out_args);
		}
		
		return res;
	}
	
	Class* Class::get_root_class()
	{
		static Class *kls = NULL;
		if(kls == NULL)
		{
			kls = new Class("<RootClass>", get_root_object());
			kls->install_method("%new", Slot::PUBLIC, new Method(&types::RootNew));
			kls->install_method("%install_instance_method", Slot::PUBLIC, new Method(&types::InstallInstanceMethod));
			kls->install_method("%install_instance_property", Slot::PUBLIC, new Method(&types::InstallInstanceProperty));
			kls->install_method("%instance_slots_list", Slot::PUBLIC, new Method(&types::EmptyConstructor));
			kls->install_method("%remove_instance_slot", Slot::PUBLIC, new Method(&types::RemoveInstanceSlot));
			kls->install_instance_method("%constructor", Slot::PUBLIC, new Method(&types::EmptyConstructor));
		}
		return kls;
	}
	
	void Class::RemoveInstanceSlot(const std::string &_slotName)
	{
		if(m_instance_slots.find(_slotName) != m_instance_slots.end())
		{
			m_instance_slots.erase(m_instance_slots.find(_slotName));
		}
		else
		{
			if(m_isa && dynamic_cast<Class*>(m_isa))
			{
				dynamic_cast<Class*>(m_isa)->RemoveInstanceSlot(_slotName);
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
		Class *kls = dynamic_cast<Class*>(ctx.self.object_ref_get());
		Instance *inst = kls->create_instance(in);
		out.push_back(ObjectMatrix::create_standard_ref(inst));
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
		Class *kls = dynamic_cast<Class*>(ctx.self.object_ref_get());
		
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
		Class *kls = dynamic_cast<Class*>(ctx.self.object_ref_get());
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
		
		kls->install_instance_property(name->string_get(0,0), svisib, def, getter, setter);
		
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
		Class *kls = dynamic_cast<Class*>(ctx.self.object_ref_get());
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
		
		kls->install_instance_method(name->string_get(0,0), svisib, func);
		
		return Callable::OK_NoResult;
	}
}
