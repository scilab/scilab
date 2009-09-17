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

#ifndef __CLASS_HXX__
#define __CLASS_HXX__

#include "object.hxx"

namespace types
{
	class Class: public Object
	{
	public:
		/* Create a new class */
		static Class *Create(const std::string &_name, Class *_pSuperClass = NULL);
	
		virtual ~Class();
		
		/* Create an instance of this class, and call the constructor */
		Instance *CreateInstance(typed_list &_constructorArgs);
	
		/* Manipulate instance slots ; see corresponding methods in Object */
		void InstallInstanceProperty(const std::string &_slotName, Slot::Visibility _visibility, InternalType *_pDefault, Callable *_pGetter, Callable *_pSetter);
		void InstallInstanceMethod(const std::string &_slotName, Slot::Visibility _visibility, Callable *_pFunc);
		void RemoveInstanceSlot(const std::string &_slotName); /* Not recursive */
		const std::map<std::string, Slot&>& GetInstanceSlots() const { return m_instanceSlots; } /* Not recursive */
		
		/* Search an instance slot, not recursive */
		Slot *ResolvInstanceSlot(const std::string &_slotName);
		
		std::string toString() const { return "<Class '"+m_name+"'>"; }
		const std::string& GetName() const { return m_name; }
		
		static Class *GetRootClass();
		
	private:
		Class(const std::string &_name, Object *_pSuper);
		Instance *CreateInstance();
	
	protected:
		std::map<std::string, Slot&> m_instanceSlots;
		std::string m_name;
	};
}

#endif /* !__CLASS_HXX__ */
