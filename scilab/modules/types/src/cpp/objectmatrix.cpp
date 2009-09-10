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
		m_optr(new Object*[rows*cols]), m_lvptr(NULL), m_startptr(NULL), m_repr(NULL)
	{
		m_iRows = rows;
		m_iCols = cols;
		m_iSize = rows * cols;
	}
		
	ObjectMatrix::~ObjectMatrix()
	{
		delete [] m_optr;
	}
	
	Object* ObjectMatrix::elem_get(int pos) const
	{
		return m_optr[pos];
	}
	
	bool ObjectMatrix::elem_set(int row, int col, Object *val)
	{
		/* Out of bounds check */
		if(row >= m_iRows || col >= m_iCols)
		{
			return false;
		}
		
		return elem_set(col * m_iRows + row, val);
	}
	
	bool ObjectMatrix::elem_set(int pos, Object *val)
	{
		/* Check element compatibility */
		if(m_repr == NULL)
		{	/* Firts element inserted */
			if(dynamic_cast<Instance*>(val) != NULL)
			{
				m_repr = dynamic_cast<Instance*>(val)->class_get();
			}
			else if(dynamic_cast<Class*>(val) != NULL)
			{
				m_repr = Class::get_root_class();
			}
			else
			{
				m_repr = val;
			}
		}
		else
		{
			if(m_repr == Class::get_root_class())
			{	/* Matrix contais classes ; we want a class */
				if(dynamic_cast<Class*>(val) == NULL)
				{
					return false;
				}
			}
			else if(m_repr == Object::get_root_object())
			{	/* Matrix contains root object ; we accept only the root object */
				if(val != Object::get_root_object())
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
				Object *root = Class::get_root_class();
				
				for(cur_lv = m_repr; cur_lv != root; cur_lv = cur_lv->super())
					super_chain_repr.push(cur_lv);
				
				for(cur_lv = val_inst->class_get(); cur_lv != root; cur_lv = cur_lv->super())
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
				
				m_repr = new_repr;
			}
		}
		
		m_optr[pos] = val;
		return true;
	}
	
	bool ObjectMatrix::insert(int row, int col, const ObjectMatrix *other)
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
				if(!elem_set(row + curRow, col + curCol, other->elem_get(curRow, curCol)))
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
			return m_optr[0]->toString();
		}
		else
		{
			std::stringstream ss;
			std::string ret;
			ss << "<" << m_iRows << "x" << m_iCols << " matrix of ";
			if(m_repr == Class::get_root_class())
			{ /* Matrix contains classes */
				ss << "classes";
			}
			else if(m_repr == Object::get_root_object())
			{ /* Matrix contains root object */
				ss << "root object";
			}
			else
			{ /* Matrix contains instances of m_repr class */
				ss << "'";
				ss << dynamic_cast<Class*>(m_repr)->name_get();
				ss << "' instances";
			}
			ss << ">";
			return ss.str();
		}
	}
	
	InternalType* ObjectMatrix::get(const std::string &p_slotName) const
	{
		ObjectMatrix *sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
		return get(p_slotName, sender);
	}
		
	InternalType* ObjectMatrix::get(const std::string &p_slotName, ObjectMatrix *p_sender) const
	{
		if(m_iSize != 1)
		{
			throw std::string("Must be a single object");
		}
		
		/* this.x, with x private */
		if(p_sender != NULL && is_this())
		{
			if(p_sender->m_lvptr == m_lvptr)
			{
				InternalType * res = m_lvptr->get_priv(p_slotName, m_optr[0], p_sender);
				if(res != NULL)
					return res;
			}
		}
		
		if(is_super())
		{	/* super.x */
			return m_startptr->get(p_slotName, m_optr[0], p_sender);
		}
		else
		{	/* normal case */
			return m_optr[0]->get(p_slotName, m_optr[0], p_sender);
		}
	}
	
	void ObjectMatrix::set(const std::string &p_slotName, InternalType * p_value) const
	{
		ObjectMatrix *sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
		return set(p_slotName, p_value, sender);
	}
		
	void ObjectMatrix::set(const std::string &p_slotName, InternalType * p_value, ObjectMatrix *p_sender) const
	{
		if(m_iSize != 1)
		{
			throw std::string("Must be a single object");
		}
		
		/* this.x, with x private */
		if(p_sender != NULL && is_this())
		{
			if(p_sender->m_lvptr == m_lvptr)
			{
				if(m_lvptr->set_priv(p_slotName, p_value, m_optr[0], p_sender))
				{
					return;
				}
			}
		}
		
		if(is_super())
		{
			/* super.x = y */
			m_startptr->set(p_slotName, p_value, m_optr[0], p_sender);
		}
		else
		{
			/* normal case */
			m_optr[0]->set(p_slotName, p_value, m_optr[0], p_sender);
		}
	}
	
	ObjectMatrix* ObjectMatrix::create_this(Object *p_self, Object *p_level)
	{
		return new ObjectMatrix(p_self, p_level, NULL);
	}
	
	ObjectMatrix* ObjectMatrix::create_super(Object *p_self, Object *p_startp)
	{
		return new ObjectMatrix(p_self, NULL, p_startp);
	}
	
	ObjectMatrix* ObjectMatrix::create_standard_ref(Object *p_self)
	{
		return new ObjectMatrix(p_self, NULL, NULL);
	}
	
	InternalType* ObjectMatrix::clone()
	{
		if(m_iSize == 1)
		{
			return ObjectMatrix::create_standard_ref(m_optr[0]);
		}
		
		ObjectMatrix *ret = new ObjectMatrix(m_iRows, m_iCols);
		ret->insert(0, 0, this);
		return ret;
	}
}
