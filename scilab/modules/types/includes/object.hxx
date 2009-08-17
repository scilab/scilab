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

#ifndef __OBJECT_HXX__
#define __OBJECT_HXX__

#include <map>
#include <string>
#include "function.hxx"

namespace types
{
  struct Slot
  {
    typedef enum {PRIVATE, PROTECTED, PUBLIC} Visibility;
    Visibility visibility;
    
    virtual ~Slot() {}
  };
  
  struct PropertySlot: public Slot
  {
    Function *getter;
    Function *setter;
    InternalType *value;
  };
  
  struct MethodSlot: public Slot
  {
    Function *func;
  };
  
  class ObjectMatrix;
  class Class;
  class Instance;
  
  class Object
  {
  friend class Class;
  
  public:
    virtual ~Object() { }
    
    InternalType *get(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender);
    InternalType *get_priv(const std::string &p_slotName, Object *p_self, ObjectMatrix *p_sender);
    void set(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender);
    bool set_priv(const std::string &p_slotName, InternalType *p_value, Object *p_self, ObjectMatrix *p_sender);
    
    void install_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Function *getter, Function *setter);
    void install_method(const std::string &p_slotName, Slot::Visibility p_visibility, Function *p_func);
    
    Object *super() { return m_isa; }
    
    virtual std::string toString() const { return "Object"; }
  
  private:
    static Object *get_root_object();
  
  protected:
    Object(Object *p_isa = NULL): m_isa(p_isa) {}
  
    /*
     * Search p_slotName in slots list (recursively).
     * If found, set r_lvptr to the level where the slot is found and r_slot to the found slot. Else, set them to NULL
     * If not found, the return value indicates wether the slot has been found but is fot visible (true) or not (false)
     */
    virtual bool resolv_slot(const std::string &p_slotName, const ObjectMatrix *p_sender,
      Object **r_lvptr, Slot **r_slot);
  
    /* Search p_slotName in local slots
     * Same rules as resolv_slot for r_slot and return value.
     */
    virtual bool resolv_slot_local(const std::string &p_slotName, const ObjectMatrix *p_sender,
      Slot **r_slot);
    
    // Returns true if p_slot (considered as a slot of this object) is visible from p_sender
    virtual bool is_visible(const Slot *p_slot, const ObjectMatrix *p_sender) const;
  
    // Perform a get operation on this slot (property: returns its value or the result of the getter, method: return the bound method)
    InternalType *do_get(Slot *p_slot, Object *p_level);
  
    // Perform a set operation on this slot (must be a property)
    void do_set(Slot *p_slot, Object *p_level, InternalType *p_value);
    
    // Call the function as a method (setting this and super). VaArgs: null terminated list of InternalType*. Returns only the first return value of the real function.
    InternalType *do_call(Function *func, Object *level,...);
    
    // Returns the bound method
    InternalType *do_bind(Function *func, Object *level);
  
  protected:
    typedef std::map<std::string, Slot&>::iterator SlotsIterator;
    typedef std::map<std::string, Slot&>::const_iterator SlotsCIterator;
  
    std::map<std::string, Slot&> m_slots;
    Object *m_isa;
  };
  
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
    
    std::string toString(int p_prec, int p_cols) const { return m_optr->toString(); }
    
    virtual ~ObjectMatrix() {}
  
  private:
    ObjectMatrix(): InternalType() { DenyDelete(); }
  
    Object *m_optr; // Reference to the object
    Object *m_lvptr; // For "this" only: pointer to current level
    Object *m_startptr; // For "super" only: pointer to beginning level
  };
  
  class Class: public Object
  {
  public:
    static Class *create(const std::string &p_name, Class *p_super_class = NULL);
    virtual ~Class() {}
  
    Instance *create_instance(typed_list &p_constructor_args);
  
    void install_instance_property(const std::string &p_slotName, Slot::Visibility p_visibility, InternalType *p_default, Function *getter, Function *setter);
    void install_instance_method(const std::string &p_slotName, Slot::Visibility p_visibility, Function *p_func);
    
    Slot *resolv_instance_slot(const std::string &p_slotName);
    
    std::string toString() const { return "<Class '"+m_name+"'>"; }
    std::string name_get() const { return m_name; }
  
  private:
    Class(const std::string &p_name, Object *p_isa);
    Instance *create_instance();
    
    static Class *get_root_class();
  
  protected:
    std::map<std::string, Slot&> m_instance_slots;
    std::string m_name;
  };
  
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
  
  class Method: public Function
  {
  public:
    typedef Function::ReturnValue (*GW_METH)(typed_list &in, int* _piRetCount, typed_list &out, ObjectMatrix *p_this, ObjectMatrix *p_super);
  
    Function::ReturnValue call(typed_list &in, int* _piRetCount, typed_list &out);
  
    Method(GW_METH p_callback): Function(), m_callback(p_callback) {}
    virtual ~Method() {}
  
  protected:
    GW_METH m_callback;
  };
  
  extern Function *ro_setter;
}

#endif /* !__OBJECT_HXX__ */
