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

#ifndef __OOUTILS_PRIVATE_HXX__
#define __OOUTILS_PRIVATE_HXX__

#include "object.hxx"

namespace types
{
	class BoundMethod: public Callable
	{
	public:
	  BoundMethod(Callable *_pFunc, Object *_pSelf, Object *_pLevel, ObjectMatrix *_pSender);
	  virtual ~BoundMethod();
	  
	  std::string toString(int _iPrecision, int _iLineLen)
	  {
		// FIXME : Return something usable
		return "FIXME : BoundMethod";
	  }
	  
	  // FIXME : Should not return NULL;
	  BoundMethod* clone() { return NULL; }
		
	  Callable::ReturnValue call(typed_list &_in, int _iRetCount, typed_list &_out);
	  virtual Callable::ReturnValue InnerCall(typed_list &_in, int _iRetCount, typed_list &_out);
	
	protected:
		Callable *m_pFunc;
		ObjectMatrix *m_pThis;
		ObjectMatrix *m_pSuper;
		ObjectMatrix *m_pSender;
	};
	
	class BoundGetter: public BoundMethod
	{
	public:
		BoundGetter(PropertySlot *_pSlot, Object *_pSelf, Object *_pLevel, ObjectMatrix *_pSender):
			BoundMethod(_pSlot->getter, _pSelf, _pLevel, _pSender), m_pSlot(_pSlot) {}
		virtual ~BoundGetter() {}
		
		Callable::ReturnValue InnerCall(typed_list &_in, int _iRetCount, typed_list &_out);
	
	protected:
		PropertySlot *m_pSlot;
	};
	
	class BoundSetter: public BoundMethod
	{
	public:
		BoundSetter(PropertySlot *_pSlot, Object *_pSelf, Object *_pLevel, ObjectMatrix *_pSender):
			BoundMethod(_pSlot->setter, _pSelf, _pLevel, _pSender), m_pSlot(_pSlot) {}
		virtual ~BoundSetter() {}
		
		Callable::ReturnValue InnerCall(typed_list &_in, int _iRetCount, typed_list &_out);
	
	protected:
		PropertySlot *m_pSlot;
	};
	
	/* Methods of root object */
	Callable::ReturnValue InstallProperty(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
	Callable::ReturnValue InstallMethod(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
	Callable::ReturnValue ObjectGet(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
	Callable::ReturnValue ObjectSet(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
	Callable::ReturnValue SlotsList(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
	Callable::ReturnValue RemoveSlot(typed_list&, int, typed_list&, const Method::MethodCallCtx &);
}

#endif /* !__OOUTILS_PRIVATE_HXX__ */
