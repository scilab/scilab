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

#include "ooutils.hxx"
#include "ooutils-private.hxx"
#include "string.hxx"
#include "context.hxx"

namespace types
{
	/*
	 * Public utils
	 */
	static Callable::ReturnValue RoSetterFunc(typed_list&, int, typed_list&, const Method::MethodCallCtx &)
	{
		throw std::string("Read-only property");
	}
	
	Callable *RoSetter = new Method(RoSetterFunc);
	
	/*
	 * Method
	 */
	Callable::ReturnValue Method::call(typed_list &in, int iRetCount, typed_list &out)
	{
		symbol::Context *ctx = symbol::Context::getInstance();
		Method::MethodCallCtx callCtx =
		{
			*dynamic_cast<ObjectMatrix*>(ctx->get(symbol::Symbol("this"))),
			*dynamic_cast<ObjectMatrix*>(ctx->get(symbol::Symbol("super"))),
			NULL
		};
		
		return call(in, iRetCount, out, callCtx);
	}
	
	/*
	 * Bound method
	 */
	BoundMethod::BoundMethod(Callable *p_func, Object *p_self, Object *p_level, ObjectMatrix *p_sender)
		: m_pFunc(p_func), m_pThis(NULL), m_pSuper(NULL), m_pSender(p_sender)
	{
		if(p_level == NULL)
		{
			p_level = p_self;
		}
		m_pThis = ObjectMatrix::CreateThis(p_self, p_level);
		m_pSuper = ObjectMatrix::CreateSuper(p_self, p_level->Super());
		m_pThis->IncreaseRef();
		m_pSuper->IncreaseRef();
	}
	
	BoundMethod::~BoundMethod()
	{
		m_pThis->DecreaseRef();
		if(m_pThis->isDeletable())
		{
			delete m_pThis;
		}
		
		m_pSuper->DecreaseRef();
		if(m_pSuper->isDeletable())
		{
			delete m_pSuper;
		}
	}
	
	Callable::ReturnValue BoundMethod::call(typed_list &in, int iRetCount, typed_list &out)
	{
		Callable::ReturnValue ret;
		symbol::Context *ctx = symbol::Context::getInstance();
		
		ctx->scope_begin();
		ctx->put(symbol::Symbol("this"), *m_pThis, false);
		ctx->put(symbol::Symbol("super"), *m_pSuper, false);
		
		ret = InnerCall(in, iRetCount, out);
		
		ctx->scope_end();
		
		return ret;
	}
	
	Callable::ReturnValue BoundMethod::InnerCall(typed_list &in, int iRetCount, typed_list &out)
	{
		Method *meth = dynamic_cast<Method*>(m_pFunc);
		if(meth == NULL)
		{
			return m_pFunc->call(in, iRetCount, out);
		}
		else
		{
			Method::MethodCallCtx ctx =
			{
				*m_pThis,
				*m_pSuper,
				m_pSender
			};
			
			return meth->call(in, iRetCount, out, ctx);
		}
	}
	
	/*
	 * BoundGetter
	 */
	Callable::ReturnValue BoundGetter::InnerCall(typed_list &in, int retCount, typed_list &out)
	{
		symbol::Context::getInstance()->put(symbol::Symbol("value"),
				* m_pThis->GetLevelRef()->RawGet(*m_pSlot));
		return BoundMethod::InnerCall(in, retCount, out);
	}
	
	/*
	 * BoundSetter
	 */
	Callable::ReturnValue BoundSetter::InnerCall(typed_list &in, int retCount, typed_list &out)
	{
		Callable::ReturnValue ret;
		symbol::Symbol valSym("value");
		
		symbol::Context::getInstance()->put(valSym, 
				* m_pThis->GetLevelRef()->RawGet(*m_pSlot));
		ret = BoundMethod::InnerCall(in, retCount, out);
		m_pThis->GetLevelRef()->RawSet(*m_pSlot, out.front());
		
		return ret;
	}
	
	/*
	 * Root object methods
	 */
	Callable::ReturnValue InstallProperty(typed_list &in, int retCount, typed_list &, const Method::MethodCallCtx &ctx)
	{
		if(in.size() == 0 || in.size() > 5)
		{
			return Callable::Error;
		}
		
		String *name = dynamic_cast<String*>(in[0]);
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
		
		ctx.self.GetObjectRef()->InstallProperty(name->string_get(0,0), svisib, def, getter, setter);
		
		return Callable::OK_NoResult;
	}
	
	Callable::ReturnValue InstallMethod(typed_list &in, int retCount, typed_list &, const Method::MethodCallCtx &ctx)
	{
		if(in.size() != 3)
		{
			return Callable::Error;
		}
		
		String *name = dynamic_cast<String*>(in[0]);
		String *visibility = dynamic_cast<String*>(in[1]);
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
		
		ctx.self.GetObjectRef()->InstallMethod(name->string_get(0,0), svisib, func);
		
		return Callable::OK_NoResult;
	}
	
	Callable::ReturnValue ObjectGet(typed_list &in, int retCount, typed_list &out, const Method::MethodCallCtx &ctx)
	{
		if(in.size() != 1)
			return Callable::Error;
		
		String *name = in[0]->getAsString();
		if(name == NULL)
			return Callable::Error;
		if(name->size_get() != 1)
			return Callable::Error;
		
		out.push_back(ctx.self.Get(name->string_get(0), ctx.pSender));
		return Callable::OK;
	}
	
	Callable::ReturnValue ObjectSet(typed_list &in, int retCount, typed_list &out, const Method::MethodCallCtx &ctx)
	{
		if(in.size() != 2)
			return Callable::Error;
		
		String *name = in[0]->getAsString();
		if(name == NULL)
			return Callable::Error;
		if(name->size_get() != 1)
			return Callable::Error;
		
		ctx.self.Set(name->string_get(0), in[1]->clone(), ctx.pSender);
		return Callable::OK_NoResult;
	}
	
	Callable::ReturnValue SlotsList(typed_list &in, int retCount, typed_list &out, const Method::MethodCallCtx &ctx)
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
		Object *lvl = ctx.super.GetObjectRef();
		
		do
		{
			for(it = lvl->GetSlots().begin() ; it != lvl->GetSlots().end() ; ++it)
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
			lvl = lvl->Super();
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

	Callable::ReturnValue RemoveSlot(typed_list &in, int retCount, typed_list &out, const Method::MethodCallCtx &ctx)
	{
		String *name = dynamic_cast<String*>(in[0]);
		
		if(name == NULL)
		{
			return Callable::Error;
		}
		
		ctx.self.GetObjectRef()->RemoveSlot(name->string_get(0,0));
		
		return Callable::OK_NoResult;
	}
}
