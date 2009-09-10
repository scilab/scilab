/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Simon LIPP
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "execvisitor.hxx"
#include "context.hxx"
#include <string>

#include "class.hxx"
#include "ooutils.hxx"

#include "yaspio.hxx"

using std::string;
using ast::Exp;
using ast::ExecVisitor;

namespace ast
{
	void ExecVisitor::visit (const ClassDec &e)
	{
		types::Class *super = NULL;
		types::Class *kls = NULL;
		slots_t::const_iterator it;
		const slots_t &slots = e.slots_get();
		
		/* Get parent class */
		if(e.super_get() != NULL)
		{
			types::ObjectMatrix *super_mat = dynamic_cast<types::ObjectMatrix*>(symbol::Context::getInstance()->get(*e.super_get()));
			if(super_mat)
				super = dynamic_cast<types::Class*>(super_mat->object_ref_get());
		}
		
		/* Create class */
		kls = types::Class::create(e.name_get()->name_get(), super);
		
		/* Create slots */
		for(it = slots.begin(); it != slots.end(); ++it)
		{
			SlotDec *sdec = *it;
			types::Slot::Visibility vis = types::Slot::PUBLIC;
			std::list<Var*>::iterator vars_it;
			std::list<Var*> vars = sdec->attributes_get().vars_get();
			bool class_slot = false;
			
			PropertyDec *pdec = dynamic_cast<PropertyDec*>(sdec);
			if(pdec != NULL)
			{	/* Property declaration */
				Callable *setter = NULL;
				InternalType *def = NULL;
				if(pdec->default_get())
				{
					ExecVisitor execMe;
					pdec->default_get()->accept(execMe);
					def = execMe.result_get();
					/* We have to keep a temporary reference on it, or it will be deleted by the destructor of execMe */
					def->IncreaseRef();
				}
				
				/* Attributes */
				for(vars_it = vars.begin(); vars_it != vars.end(); ++vars_it)
				{
					std::string attr = dynamic_cast<SimpleVar*>(*vars_it)->name_get().name_get();
					if(attr == "public")
						vis = types::Slot::PUBLIC;
					else if(attr == "protected")
						vis = types::Slot::PROTECTED;
					else if(attr == "private")
						vis = types::Slot::PRIVATE;
					else if(attr == "ro")
						setter = ro_setter;
					else if(attr == "cls")
						class_slot = true;
				}
				
				/* Add it */
				if(class_slot)
				{
					kls->install_property(sdec->name_get().name_get(), vis, def, NULL, setter);
				}
				else
				{
					kls->install_instance_property(sdec->name_get().name_get(), vis, def, NULL, setter);
				}
				
				if(def)
				{	
					def->DecreaseRef();
				}
			}
			else
			{	/* Method declaration */
				MethodDec *mdec = dynamic_cast<MethodDec*>(sdec);
				bool getter = false;
				bool setter = false;
				
				/* Input/output arguments */
				std::list<symbol::Symbol> &in = *new std::list<symbol::Symbol>;
				std::list<symbol::Symbol> &out = *new std::list<symbol::Symbol>;
				std::list<Var*>::const_iterator it;
				
				for(it = mdec->args_get().vars_get().begin(); it != mdec->args_get().vars_get().end(); ++it)
				{
					in.push_back(dynamic_cast<const SimpleVar*>(*it)->name_get());
				}
				
				for(it = mdec->returns_get().vars_get().begin(); it != mdec->returns_get().vars_get().end(); ++it)
				{
					out.push_back(dynamic_cast<const SimpleVar*>(*it)->name_get());
				}
				
				/* Macro */
				Callable *code = new types::Macro(sdec->name_get().name_get() + "::" + e.name_get()->name_get(),
					in, out, (SeqExp&)mdec->body_get(), "script");
				
				/* Attributes */
				for(vars_it = vars.begin(); vars_it != vars.end(); ++vars_it)
				{
					std::string attr = dynamic_cast<SimpleVar*>(*vars_it)->name_get().name_get();
					if(attr == "public")
						vis = types::Slot::PUBLIC;
					else if(attr == "protected")
						vis = types::Slot::PROTECTED;
					else if(attr == "private")
						vis = types::Slot::PRIVATE;
					else if(attr == "getter")
						getter = true;
					else if(attr == "setter")
						setter = true;
					else if(attr == "cls")
						class_slot = true;
				}
				
				if(getter || setter)
				{	/* Accessor */
					std::string slotName = sdec->name_get().name_get();
					Slot *mslot = NULL;
					
					/* Get related slot */
					if(class_slot)
					{
						if(kls->GetSlots().find(slotName) != kls->GetSlots().end())
						{
							mslot = &kls->GetSlots().find(slotName)->second;
						}
					}
					else
					{
						if(kls->GetInstanceSlots().find(slotName) != kls->GetInstanceSlots().end())
						{
							mslot = &kls->GetInstanceSlots().find(slotName)->second;
						}
					}
					
					PropertySlot *slot = dynamic_cast<PropertySlot*>(mslot);
					if(slot == NULL)
					{
						std::stringstream ss;
						ss << "Error: slot " << sdec->name_get().name_get() << " not found.";
						throw ss.str();
					}
					
					/* Add it */
					if(getter)
					{
						slot->getter = code;
					}
					else
					{
						slot->setter = code;
					}
				}
				else
				{	/* Standard method, add it */
					if(class_slot)
					{
						kls->install_method(sdec->name_get().name_get(), vis, code);
					}
					else
					{
						kls->install_instance_method(sdec->name_get().name_get(), vis, code);
					}
				}
			}
		}
		
		ObjectMatrix *res = types::ObjectMatrix::create_standard_ref(kls);
		result_set(res);
		symbol::Context::getInstance()->put(symbol::Symbol(*e.name_get()), *res);
	}
}
