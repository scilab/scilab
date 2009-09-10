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

#ifndef __CLASS_HXX__
#define __CLASS_HXX__

#include "object.hxx"

namespace types
{
  class Class: public Object
  {
  public:
    static Class *create(const std::string &p_name, Class *p_super_class = NULL);
    virtual ~Class() {}
  
    Instance *create_instance(typed_list &p_constructor_args);
  
    void install_instance_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Callable *getter, Callable *setter);
    void install_instance_method(const std::string &p_slotName, Slot::Visibility p_visibility, Callable *p_func);
    
    Slot *resolv_instance_slot(const std::string &p_slotName);
    
    std::string toString() const { return "<Class '"+m_name+"'>"; }
    std::string name_get() const { return m_name; }
  
    static Class *get_root_class();
    
    const std::map<std::string, Slot&>& GetInstanceSlots() const { return m_instance_slots; }
    
  private:
    Class(const std::string &p_name, Object *p_isa);
    Instance *create_instance();
  
  protected:
    std::map<std::string, Slot&> m_instance_slots;
    std::string m_name;
  };
}

#endif /* !__CLASS_HXX__ */
