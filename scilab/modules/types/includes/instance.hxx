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

#ifndef __INSTANCE_HXX__
#define __INSTANCE_HXX__

#include "class.hxx"

namespace types
{
  class Instance: public Object
  {
  friend class Class;
  
  public:
    virtual ~Instance() {}
    std::string toString() const { return "<'"+m_kls->name_get()+"' Instance>"; }
  
  private:
    static Instance *get_root_instance();
  
  protected:
    Instance(Class *p_kls, Object *p_isa);
    
    virtual bool resolv_slot_local(const std::string &p_slotName, const ObjectMatrix *p_sender,
      Slot **r_slot);
  
  protected:
    Class *m_kls;
  };
}

#endif /* !__INSTANCE_HXX__ */
