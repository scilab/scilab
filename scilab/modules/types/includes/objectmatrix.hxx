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

#ifndef __OBJECTMATRIX_HXX__
#define __OBJECTMATRIX_HXX__

#include "object.hxx"

namespace types
{
	class ObjectMatrix: public GenericType
	{
	public:
		ObjectMatrix(int p_rows, int p_cols);
		virtual ~ObjectMatrix();
	
		Object *GetElem(int pos) const { return m_pObj[pos]; }
		Object *GetElem(int row, int col) const { return GetElem(col * m_iRows + row); }
		Object **GetElems() { return m_pObj; }
		bool SetElem(int pos, Object *val);
		bool SetElem(int row, int col, Object *val);
		bool Insert(int row, int col,  ObjectMatrix *other);
	
		std::string toString(int, int);
		void whoAmI() { std::cout << "types::Object"; }
		bool isAssignable() { return true; }
		RealType getType() { return RealObject; }
		ObjectMatrix *getAsObject() { return this; }
		const ObjectMatrix *getAsObject() const { return this; }
		
		// obj.foo
		// Garbage collection rules are the same that Object::Get
		InternalType *Get(const std::string &p_slotName) const;
		InternalType *Get(const std::string &p_slotName, ObjectMatrix *p_sender) const;
		
		// obj.foo = bar
		// Garbage collection rules are the same that Object::Set
		void Set(const std::string &p_slotName, InternalType * p_value) const;
		void Set(const std::string &p_slotName, InternalType * p_value, ObjectMatrix *p_sender) const;
		
		Object *GetObjectRef() const { return m_pObj[0]; }
		Object *GetLevelRef() const { return m_pLvl; }
		Object *GetStartRef() const { return m_pStart; }
		
		static ObjectMatrix *CreateThis(Object *_pSelf, Object *_pLevel) { return new ObjectMatrix(_pSelf, _pLevel, NULL); }
		static ObjectMatrix *CreateSuper(Object *_pSelf, Object *_pStart) { return new ObjectMatrix(_pSelf, NULL, _pStart); }
		static ObjectMatrix *CreateStandardRef(Object *_pSelf) { return new ObjectMatrix(_pSelf, NULL, NULL); }
		
		bool IsThis() const { return m_pLvl != NULL; }
		bool IsSuper() const { return m_pStart != NULL; }
		bool IsStandardRef() const { return m_iSize == 1 && m_pLvl == NULL && m_pStart == NULL; }
		
		InternalType *clone();
		
	private:
		ObjectMatrix(Object *_pObj, Object *_pLevel, Object *_pStart):
			GenericType(), 
			m_pObj(new Object*[1]), m_pLvl(_pLevel), m_pStart(_pStart), m_pRepr(NULL)
		{ m_iRows = m_iCols = m_iSize = 1; m_pObj[0] = NULL; SetElem(0, _pObj); }
	
		Object **m_pObj; // Reference to the object
		Object *m_pLvl; // For "this" only: pointer to current level
		Object *m_pStart; // For "super" only: pointer to beginning level
		
		// Representative of the elements of the matrix
		//	If the matrix contains only the root object, it is the root object
		//	If the matrix contains classes, it is the root class
		//	If the matrix contains instances, it is the most specific common class
		//		of all contained instances. Can’t be the root class.
		Object *m_pRepr;
	};
}

#endif /* !__OBJECTMATRIX_HXX__ */
