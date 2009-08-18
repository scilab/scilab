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

#ifndef __OBJECTMATRIX_HXX__
#define __OBJECTMATRIX_HXX__

#include "object.hxx"

namespace types
{
  class ObjectMatrix: public InternalType
  {
  public:
    void whoAmI() { std::cout << "types::Object"; }
    bool isAssignable() { return true; }
    RealType getType() { return RealObject; }
    
    // obj.foo
    InternalType *get(const std::string &p_slotName);
    
    // obj.foo = bar
    void set(const std::string &p_slotName, InternalType * p_value);
    
    Object *object_ref_get() { return m_optr; }
    Object *level_ref_get() { return m_lvptr; }
    Object *start_ref_get() { return m_startptr; }
    
    const Object *object_ref_get() const { return m_optr; }
    const Object *level_ref_get() const { return m_lvptr; }
    const Object *start_ref_get() const { return m_startptr; }
    
    static ObjectMatrix *create_this(Object *p_self, Object *p_level);
    static ObjectMatrix *create_super(Object *p_self, Object *p_startp);
    static ObjectMatrix *create_standard_ref(Object *p_self);
    
    bool is_this() { return m_lvptr != NULL; }
    bool is_super() { return m_startptr != NULL; }
    bool is_standard_ref() { return m_lvptr == NULL && m_startptr == NULL; }
    
    std::string toString(int, int) const { return m_optr->toString(); }
    
    virtual ~ObjectMatrix() {}
  
  private:
    ObjectMatrix(): InternalType() { }
  
    Object *m_optr; // Reference to the object
    Object *m_lvptr; // For "this" only: pointer to current level
    Object *m_startptr; // For "super" only: pointer to beginning level
  };
}

#endif /* !__OBJECTMATRIX_HXX__ */
