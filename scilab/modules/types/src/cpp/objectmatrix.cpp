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

#include "objectmatrix.hxx"
#include "class.hxx"
#include "instance.hxx"
#include "context.hxx"
#include <stack>

namespace types
{
	ObjectMatrix::ObjectMatrix(int rows, int cols):
		GenericType(),
		m_pObj(new Object*[rows*cols]), m_pLvl(NULL), m_pStart(NULL), m_pRepr(NULL)
	{
		m_iRows = rows;
		m_iCols = cols;
		m_iSize = rows * cols;
		for(int i = 0; i < rows * cols; ++i)
		{
			m_pObj[i] = NULL;
		}
	}
		
	ObjectMatrix::~ObjectMatrix()
	{
		for(int i = 0; i < m_iSize; ++i)
		{
			m_pObj[i]->Release();
		}
		delete [] m_pObj;
	}
	
	bool ObjectMatrix::SetElem(int row, int col, Object *val)
	{
		/* Out of bounds check */
		if(row >= m_iRows || col >= m_iCols)
		{
			return false;
		}
		
		return SetElem(col * m_iRows + row, val);
	}
	
	bool ObjectMatrix::SetElem(int pos, Object *val)
	{
		/* Check element compatibility */
		if(m_pRepr == NULL)
		{	/* Firts element inserted */
			if(dynamic_cast<Instance*>(val) != NULL)
			{
				m_pRepr = dynamic_cast<Instance*>(val)->GetClass();
			}
			else if(dynamic_cast<Class*>(val) != NULL)
			{
				m_pRepr = Class::GetRootClass();
			}
			else
			{
				m_pRepr = val;
			}
		}
		else
		{
			if(m_pRepr == Class::GetRootClass())
			{	/* Matrix contais classes ; we want a class */
				if(dynamic_cast<Class*>(val) == NULL)
				{
					return false;
				}
			}
			else if(m_pRepr == Object::GetRootObject())
			{	/* Matrix contains root object ; we accept only the root object */
				if(val != Object::GetRootObject())
				{
					return false;
				}
			}
			else
			{	/* Matrix contains instances, we want a compatible instance */
				Instance *val_inst = dynamic_cast<Instance*>(val);
				if(val_inst == NULL)
				{
					return false;
				}
				
				/* Build super chain of representative and new value classes */
				std::stack<Object*> super_chain_repr;
				std::stack<Object*> super_chain_val;
				Object *cur_lv;
				Object *root = Class::GetRootClass();
				
				for(cur_lv = m_pRepr; cur_lv != root; cur_lv = cur_lv->Super())
					super_chain_repr.push(cur_lv);
				
				for(cur_lv = val_inst->GetClass(); cur_lv != root; cur_lv = cur_lv->Super())
					super_chain_val.push(cur_lv);
				
				/* Find the innermost common object in the stacks ; it is the new representative */
				Object *new_repr = NULL;
				while(!super_chain_val.empty() && !super_chain_repr.empty() && super_chain_val.top() == super_chain_repr.top())
				{
					new_repr = super_chain_val.top();
					super_chain_val.pop();
					super_chain_repr.pop();
				}
				
				/* Check types compatibility */
				if(new_repr == NULL)
				{
					return false;
				}
				
				m_pRepr = new_repr;
			}
		}
		
		if(m_pObj[pos] != NULL)
		{
			m_pObj[pos]->Release();
		}
		m_pObj[pos] = val;
		val->Retain();
		return true;
	}
	
	bool ObjectMatrix::Insert(int row, int col, ObjectMatrix *other)
	{
		/* Out of bounds check */
		if(row + other->rows_get() > m_iRows || col + other->cols_get() > m_iCols)
		{
			return false;
		}
		
		/* Add element, cell by cell */
		int curRow, curCol;
		for(curCol = 0 ; curCol < other->cols_get() ; ++curCol)
		{
			for(curRow = 0 ; curRow < other->rows_get() ; ++curRow)
			{
				if(!SetElem(row + curRow, col + curCol, other->GetElem(curRow, curCol)))
				{
					return false;
				}
			}
		}
		
		return true;
	}
	
	std::string ObjectMatrix::toString(int, int)
	{
		if(m_iSize == 1)
		{
			return m_pObj[0]->toString();
		}
		else
		{
			std::stringstream ss;
			std::string ret;
			ss << "<" << m_iRows << "x" << m_iCols << " matrix of ";
			if(m_pRepr == Class::GetRootClass())
			{ /* Matrix contains classes */
				ss << "classes";
			}
			else if(m_pRepr == Object::GetRootObject())
			{ /* Matrix contains root object */
				ss << "root object";
			}
			else
			{ /* Matrix contains instances of m_pRepr class */
				ss << "'";
				ss << dynamic_cast<Class*>(m_pRepr)->GetName();
				ss << "' instances";
			}
			ss << ">";
			return ss.str();
		}
	}
	
	InternalType* ObjectMatrix::Get(const std::string &p_slotName) const
	{
		ObjectMatrix *sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
		return Get(p_slotName, sender);
	}
		
	InternalType* ObjectMatrix::Get(const std::string &p_slotName, ObjectMatrix *p_sender) const
	{
		if(m_iSize != 1)
		{
			throw std::string("Must be a single object");
		}
		
		/* this.x, with x private */
		if(p_sender != NULL && IsThis())
		{
			if(p_sender->m_pLvl == m_pLvl)
			{
				InternalType * res = m_pLvl->GetPriv(p_slotName, m_pObj[0], p_sender);
				if(res != NULL)
					return res;
			}
		}
		
		if(IsSuper())
		{	/* super.x */
			return m_pStart->Get(p_slotName, m_pObj[0], p_sender);
		}
		else
		{	/* normal case */
			return m_pObj[0]->Get(p_slotName, m_pObj[0], p_sender);
		}
	}
	
	void ObjectMatrix::Set(const std::string &p_slotName, InternalType * p_value) const
	{
		ObjectMatrix *sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
		return Set(p_slotName, p_value, sender);
	}
		
	void ObjectMatrix::Set(const std::string &p_slotName, InternalType * p_value, ObjectMatrix *p_sender) const
	{
		if(m_iSize != 1)
		{
			throw std::string("Must be a single object");
		}
		
		/* this.x, with x private */
		if(p_sender != NULL && IsThis())
		{
			if(p_sender->m_pLvl == m_pLvl)
			{
				if(m_pLvl->SetPriv(p_slotName, p_value, m_pObj[0], p_sender))
				{
					return;
				}
			}
		}
		
		if(IsSuper())
		{
			/* super.x = y */
			m_pStart->Set(p_slotName, p_value, m_pObj[0], p_sender);
		}
		else
		{
			/* normal case */
			m_pObj[0]->Set(p_slotName, p_value, m_pObj[0], p_sender);
		}
	}
	
	InternalType* ObjectMatrix::clone()
	{
		/* the copy of "this" and "super" are standard references */
		if(m_iSize == 1)
		{
			return ObjectMatrix::CreateStandardRef(m_pObj[0]);
		}
		
		ObjectMatrix *ret = new ObjectMatrix(m_iRows, m_iCols);
		ret->Insert(0, 0, this);
		return ret;
	}
}
