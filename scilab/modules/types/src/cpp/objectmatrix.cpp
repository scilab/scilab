/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Simon LIPP
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "objectmatrix.hxx"
#include "context.hxx"

namespace types
{
  InternalType *
  ObjectMatrix::get(const std::string &p_slotName)
  {
    ObjectMatrix *sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
    
    // Handle the case of this.x with x being private
    if(sender != NULL && is_this())
    {
      if(sender->m_lvptr == m_lvptr)
      {
        InternalType * res = m_lvptr->get_priv(p_slotName, m_optr, sender);
        if(res != NULL)
          return res;
      }
    }
    
    if(is_super())
      // super.x
      return m_startptr->get(p_slotName, m_optr, sender);
    else
      // normal case
      return m_optr->get(p_slotName, m_optr, sender);
  }
  
  void
  ObjectMatrix::set(const std::string &p_slotName, InternalType * p_value)
  {
    ObjectMatrix * sender = dynamic_cast<ObjectMatrix*>(symbol::Context::getInstance()->get(symbol::Symbol("this")));
    
    // Handle the case of this.x = y with x being private
    if(sender != NULL && is_this())
      if(sender->m_lvptr == m_lvptr)
        if(m_lvptr->set_priv(p_slotName, p_value, m_optr, sender))
          return;
    
    if(is_super())
      // super.x = y
      m_startptr->set(p_slotName, p_value, m_optr, sender);
    else
      // normal case
      m_optr->set(p_slotName, p_value, m_optr, sender);
  }
  
  ObjectMatrix *
  ObjectMatrix::create_this(Object *p_self, Object *p_level)
  {
    ObjectMatrix *res = new ObjectMatrix;
    res->m_optr = p_self;
    res->m_lvptr = p_level;
    res->m_startptr = NULL;
    return res;
  }
  
  ObjectMatrix *
  ObjectMatrix::create_super(Object *p_self, Object *p_startp)
  {
    ObjectMatrix *res = new ObjectMatrix;
    res->m_optr = p_self;
    res->m_lvptr = NULL;
    res->m_startptr = p_startp;
    return res;
  }
  
  ObjectMatrix *
  ObjectMatrix::create_standard_ref(Object *p_self)
  {
    ObjectMatrix *res = new ObjectMatrix;
    res->m_optr = p_self;
    res->m_lvptr = NULL;
    res->m_startptr = NULL;
    return res;
  }
}
