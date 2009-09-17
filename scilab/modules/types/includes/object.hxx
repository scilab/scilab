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

#ifndef __OBJECT_HXX__
#define __OBJECT_HXX__

#include <map>
#include <string>
#include "callable.hxx"

namespace types
{
	/** Represents a slot of an object
	* It belongs to the object; that is, is destroyed with the object owning it.
	*/
	struct Slot
	{
		typedef enum {PRIVATE, PROTECTED, PUBLIC} Visibility;
		std::string name;
		Visibility visibility;
		
		Slot(): name(), visibility(PUBLIC) {}
		virtual ~Slot() {}
	};
	
	/** Represents a property
	* getter and setter can be set to NULL, default_value can't.
	* All are destroyed with the property.
	*/
	struct PropertySlot: public Slot
	{
		Callable *getter;
		Callable *setter;
		InternalType *default_value;
		
		PropertySlot(): Slot(), getter(NULL), setter(NULL), default_value(NULL) {}
		virtual ~PropertySlot();
	};
	
	/** Represents a method
	* func is destroyed with the slot.
	*/
	struct MethodSlot: public Slot
	{
		Callable *func;
		
		MethodSlot(): Slot(), func(NULL) {}
		virtual ~MethodSlot();
	};
	
	/** Declarations to avoid circular references */
	class ObjectMatrix;
	class Instance;
	
	/** Represents an object
	*	 It contains slots, which are destroyed with the object (right now, when
	* reference count is 0).
	*	 It must not be confused with ObjectMatrix, which is a reference on one
	* (or several) objects in the scilab environment.
	*/
	class Object
	{
	public:
		virtual ~Object();
		
		/** Perform (this object).(slot) ; returns NULL if the slot isn't found.
		*   p_sender is the caller object reference, that is, the "this" variable
		* in the calling environment, if the calling environment is a method (NULL
		* otherwise)
		*   p_self is the current object (the bottom in the super chain). It can
		* be different in the case of the special variable super, where the slot
		* resolution algorithm starts from a level upper than the bottom level in
		* the super chain. If this case, this is the start level of slot
		* resolution while p_self is the bottom of the super chain.
		*   This object has a reference (IncreaseRef) on the result. The result
		* will be destroyed with this object if it is deletable.
		*/
		InternalType *Get(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender);
		
		/** Perform (this object).(slot), private slot case
		*	 See Object::get for general discussion of arguments.
		*	 The resolution of a private slot is a bit different. If the sender is
		* "this", then the private slot must be choosen even if it is overloaded
		* somewhere else in the super chain.
		*	 Note that no visibility check is performed ; it is up to the user of
		* this method to check that "sender" is the special variable "this"
		* refering to this object. See ObjectMatrix::get.
		*/
		InternalType *GetPriv(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender);
		
		/** Perform (this.object).(slot) = (value).
		*	 See Object::get for considerations on arguments.
		*	 No copy is performed on p_value, but a IncreaseRef is.
		*/
		void Set(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender);
		
		/** Perform (this.object).(slot) = (value), private case.
		*	 See Object::set and Object::get_priv for general considerations.
		*/
		bool SetPriv(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender);
		
		/** Install a property on the object.
		*   getter and setter can be NULL ; p_default can’t.
		*   getter, setter and p_default will not be cloned but an IncreaseRef
		* will be performed on it. They will be destroyed with this object if
		* they are deletable.
		*/
		void InstallProperty(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *getter, Callable *setter);
		
		/** Install a method on the object.
		*   funct will not be cloned but an IncreaseRef will be performed on it.
		*   It will be destroyed with this object if they are deletable.
		*/
		void InstallMethod(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func);
		
		/** Remove the first encountered slot by this name */
		void RemoveSlot(const std::string &_slotName);
		
		/** Get the parent object */
		Object *Super() { return m_pSuper; }
		
		/** See InternalType::toString */
		virtual std::string toString() const { return "<Root object>"; }
		
		/** Get slots */
		const std::map<std::string, Slot&>& GetSlots() const { return m_slots; }
	
		// root_object is the object at the top of all super chains
		static Object *GetRootObject();
	
		// m_slots_values manipulation
		InternalType *RawGet(PropertySlot &slot);
		void RawSet(PropertySlot &slot, InternalType *value);
		
		/* Garbage collection
		* Right now, simple refcount with no cycle detection. A created object begins with
		* a refcount of 1 ; it is destroyed as soon as its refcount falls to 0
		* Since it’s a bit different from the refcount of InternalType, name it differently
		*/
		inline Object* Retain() { ++m_iRefCount; return this; }
		inline void Release() { if(--m_iRefCount == 0) delete this; }
	
	protected:
		Object(Object *_pSuper = NULL):
			m_pSuper(_pSuper), m_slots(), m_slotsValues(), m_iRefCount(1)
		{ if(_pSuper) _pSuper->Retain(); }
	
		// Search p_slotName in slots list (recursively).
		// If found, set r_lvptr to the level where the slot is found and r_slot to
		//	the found slot. Else, set them to NULL
		// If not found, the return value indicates wether the slot has been found 
		//	but is fot visible (true) or not (false)
		virtual bool ResolvSlot(const std::string &p_slotName, const ObjectMatrix *p_sender,
			Object **r_lvptr, Slot **r_slot);
	
		// Search p_slotName in local slots. Same rules as resolv_slot for r_slot
		// and return value.
		virtual bool ResolvSlotLocal(const std::string &p_slotName, const ObjectMatrix *p_sender,
			Slot **r_slot);
		
		// Returns true if p_slot (considered as a slot of this object) is visible
		// from p_sender
		virtual bool IsVisible(const Slot *p_slot, const ObjectMatrix *p_sender) const;
	
		// Perform a get operation on this slot (property: returns its value or
		// the result of the getter, method: return the bound method)
		InternalType *DoGet(Slot *p_slot, Object *p_level, ObjectMatrix *p_sender);
	
		// Perform a set operation on this slot (must be a property)
		void DoSet(Slot *p_slot, Object *p_level, InternalType *p_value, ObjectMatrix *p_sender);
		
		// Call the function. VaArgs: null terminated list of InternalType*. 
		// Returns only the first return value of the function.
		InternalType *DoCall(Callable &func, int retCount, ...);
	
	protected:
		typedef std::map<std::string, Slot&>::iterator SlotsIterator;
		typedef std::map<std::string, Slot&>::const_iterator SlotsCIterator;
		
		// We split apart the slot (including its default value) and its value
		// since an instance slot may be shared among many objects, but its
		// value is still specific to each object
		std::map<std::string, InternalType *> m_slotsValues;
		std::map<std::string, Slot&> m_slots;
		Object *m_pSuper;
		int m_iRefCount;
	};
}

#endif /* !__OBJECT_HXX__ */
