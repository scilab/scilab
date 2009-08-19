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

#include "instance.hxx"
#include "objectmatrix.hxx"
#include "ooutils.hxx"

namespace types
{
  Instance::Instance(Class *p_kls, Object *p_isa):
    Object(p_isa),
    m_kls(p_kls)
  {
    install_property("super", Slot::PRIVATE, ObjectMatrix::create_standard_ref(p_isa), NULL, ro_setter);
    install_property("klass", Slot::PUBLIC, ObjectMatrix::create_standard_ref(p_kls), NULL, ro_setter);
    install_property("level_class", Slot::PRIVATE, ObjectMatrix::create_standard_ref(p_kls), NULL, ro_setter);
  }
  
  bool
  Instance::resolv_slot_local(const std::string &p_slotName, const ObjectMatrix *p_sender,
      Slot **r_slot)
  {
    bool res = Object::resolv_slot_local(p_slotName, p_sender, r_slot);
    if(*r_slot != NULL)
      return res;
    
    Slot *slot = m_kls->resolv_instance_slot(p_slotName);
    if(slot != NULL)
    {
      if(is_visible(slot, p_sender))
        *r_slot = slot;
      return true;
    }
    
    return false;
  }
}
