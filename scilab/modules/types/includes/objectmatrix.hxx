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
  class ObjectMatrix: public GenericType
  {
  public:
    ObjectMatrix(int p_rows, int p_cols);
    virtual ~ObjectMatrix();
  
    Object *elem_get(int pos) const;
    Object *elem_get(int row, int col) const { return elem_get(col * m_iRows + row); }
    Object **elems_get() { return m_optr; }
    bool elem_set(int pos, Object *val);
    bool elem_set(int row, int col, Object *val);
    bool insert(int row, int col, const ObjectMatrix *other);
  
    std::string toString(int, int);
    void whoAmI() { std::cout << "types::Object"; }
    bool isAssignable() { return true; }
    RealType getType() { return RealObject; }
    ObjectMatrix *getAsObject() { return this; }
    const ObjectMatrix *getAsObject() const { return this; }
    
    // obj.foo
    InternalType *get(const std::string &p_slotName) const;
    
    // obj.foo = bar
    void set(const std::string &p_slotName, InternalType * p_value) const;
    
    Object *object_ref_get() const { return m_optr[0]; }
    Object *level_ref_get() const { return m_lvptr; }
    Object *start_ref_get() const { return m_startptr; }
    
    static ObjectMatrix *create_this(Object *p_self, Object *p_level);
    static ObjectMatrix *create_super(Object *p_self, Object *p_startp);
    static ObjectMatrix *create_standard_ref(Object *p_self);
    
    bool is_this() const { return m_lvptr != NULL; }
    bool is_super() const { return m_startptr != NULL; }
    bool is_standard_ref() const { return m_iSize == 1 && m_lvptr == NULL && m_startptr == NULL; }
    
  private:
    ObjectMatrix(Object *p_optr, Object *p_lvptr, Object *p_startptr):
      GenericType(), 
      m_optr(new Object*[1]), m_lvptr(p_lvptr), m_startptr(p_startptr), m_repr(NULL)
    { m_iRows = m_iCols = m_iSize = 1; elem_set(0, 0, p_optr); }
  
    Object **m_optr; // Reference to the object
    Object *m_lvptr; // For "this" only: pointer to current level
    Object *m_startptr; // For "super" only: pointer to beginning level
    
    // Representative of the elements of the matrix
    //  If the matrix contains only the root object, it is the root object
    //  If the matrix contains classes, it is the root class
    //  If the matrix contains instances, it is the most specific common class
    //    of all contained instances. Can’t be the root class.
    Object *m_repr;
  };
}

#endif /* !__OBJECTMATRIX_HXX__ */
