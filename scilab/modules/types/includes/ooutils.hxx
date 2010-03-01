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

#ifndef __OOUTILS_HXX__
#define __OOUTILS_HXX__

#include "objectmatrix.hxx"

namespace types
{
	class Method: public Callable
	{
	public:
		struct MethodCallCtx
		{
			ObjectMatrix &self;		/* = this in the Scilab environnment */
			ObjectMatrix &super;	 /* = super in the Scilab environnment */
			ObjectMatrix *pSender; /* = the object calling this method, or NULL if not called from a metho */
		};
		
		typedef Callable::ReturnValue (*GW_METH)(typed_list &_in, int _iRetCount, typed_list &_out, const MethodCallCtx &_ctx);
	
	  std::string toString(int _iPrecision, int _iLineLen)
	  {
		// FIXME : Return something usable
		return "FIXME : Method";
	  }
	  
	  // FIXME : Should not return NULL;
	  Method* clone() { return NULL; }

		Callable::ReturnValue call(typed_list &_in, int _iRetCount, typed_list &_out);
		Callable::ReturnValue call(typed_list &_in, int _iRetCount, typed_list &_out, const MethodCallCtx &_ctx)
		{
			return m_pCallback(_in, _iRetCount, _out, _ctx);
		}
	
		Method(GW_METH _pCallback): Callable(), m_pCallback(_pCallback) {}
		virtual ~Method() {}
	
	protected:
		GW_METH m_pCallback;
	};
	
	extern Callable *RoSetter;
}

#endif /* !__OOUTILS_HXX__ */
