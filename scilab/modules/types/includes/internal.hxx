/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#ifndef __INTERNAL_HXX__
#define __INTERNAL_HXX__

#include <vector>
#include <iostream>
#include <string.h>
#include "anytype.hxx"


namespace types
{
  class InternalType
  {
  public :
    enum RealType {
      /* Internal Type */
      RealInternal,
      /* Generic Types */
      RealGeneric,
      RealInt,
      RealString,
      RealDouble,
      RealBool,
      RealFloat,
      RealPoly,
      RealSinglePoly,
      /* Callable */
      RealFunction,
      RealMacro,
      RealMacroFile,
      /* Implicit List */
      RealImplicitList,
      /* User */
      RealUserType
    };
    
  protected :
    InternalType() {
      m_iRef = 0;
      m_bAllowDelete = true;
    }

  public :
    virtual ~InternalType(){};
    virtual void whoAmI(void) { std::cout << "types::Inernal"; }

    virtual bool isAssignable(void) { return false; }
    virtual RealType getType(void) { return RealInternal; }

    virtual std::string	toString(int _iPrecison, int _iLineLen) { return "Internal"; }
    virtual InternalType* clone(void) { return NULL; }
    
    void	IncreaseRef()
    {
      m_iRef++;
    }
    
    void	DecreaseRef()
    {
      if(m_iRef > 0)
	{
	  m_iRef--;
	}
    }

    bool	isDeletable() { return m_iRef == 0; }
    bool	isRef(int _iRef = 0) { return m_iRef > _iRef; }

    virtual bool operator==(const InternalType& it) { return (getType() == ((InternalType*)&it)->getType()); }
    virtual bool operator!=(const InternalType& it) { return !(*this == it); }

    /**
     ** GenericType
     ** \{
     */

    /* String */
    bool isString(void) { return (getType() == RealString); }
    virtual String* getAsString(void) { return NULL; }

    /* Double */
    bool isDouble(void) { return (getType() == RealDouble); }
    virtual Double* getAsDouble(void) { return NULL; }

    /* Float */
    bool isFloat(void) { return (getType() == RealFloat); }
    virtual Float* getAsFloat(void) { return NULL; }

    /* Int */
    bool isInt(void) { return (getType() == RealInt); }
    virtual Int* getAsInt(void) { return NULL; }

    /* Bool */
    bool isBool(void) { return (getType() == RealBool); }
    virtual Bool* getAsBool(void) { return NULL; }
    
    /* Poly Matrix */
    bool isPoly(void) { return (getType() == RealPoly); }
    virtual MatrixPoly* getAsPoly(void) { return NULL; }

    /* Single Poly */
    bool isSinglePoly(void) { return (getType() == RealSinglePoly); }
    virtual Poly* getAsSinglePoly(void) { return NULL; }
    
    /**
     ** \}
     */

    /**
     ** Callable 
     ** \{
     */
    virtual bool isCallable(void) { return false; }
    virtual Callable* getAsCallable(void) { return NULL; }

    /* Function */
    bool isFunction(void) { return (getType() == RealFunction); }
    virtual Function* getAsFunction(void) { return NULL; }

    /* Macro */
    bool isMacro(void) { return (getType() == RealMacro); }
    virtual Macro* getAsMacro(void) { return NULL; }

    /* MacroFile */
    bool isMacroFile(void) { return (getType() == RealMacroFile); }
    virtual MacroFile* getAsMacroFile(void) { return NULL; }
    
    /**
     ** \}
     */

    /* ImplicitList */
    bool isImplicitList(void) { return (getType() == RealImplicitList); }
    virtual ImplicitList* getAsImplicitList(void) { return NULL; }

  private :
    int m_iRef;
    bool m_bAllowDelete; //use to know if we can delete this variables or if it's link to a scilab variable.
  };
  
  /*
  ** List of types
  */
  typedef std::vector<InternalType *> typed_list;

}
#endif /* !__INTERNAL_HXX__ */
