/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_HH__
#define __TYPES_HH__

#include <vector>
#include <iostream>
#include <string>
#include "internal.hxx"

using namespace std;

namespace types
{
  /*
  ** Type
  */
	class GenericType : public InternalType
  {

	protected :
		int m_iRows;
		int m_iCols;
		int m_iSize;

  public :

  protected :
    GenericType() {}
		virtual ~GenericType() {}
  public :
    void whoAmI(void) { std::cout << "types::GenericType"; }

		/*sub Type management*/
    /* String */
    bool isString(void) { return (getType() == RealString); }

    /* Double */
    bool isDouble(void) { return (getType() == RealDouble); }

    /* Float */
    bool isFloat(void) { return (getType() == RealFloat); }

    /* Int */
    bool isInt(void) { return (getType() == RealInt); }

    /* Int */
    bool isInt2(void) { return (getType() == RealInt2); }

    /* Bool */
    bool isBool(void) { return (getType() == RealBool); }

    /* Function */
    bool isFunction(void) { return (getType() == RealFunction); }

    /* Macro */
    bool isMacro(void) { return (getType() == RealMacro); }

    /* MacroFile */
		bool isMacroFile(void) { return (getType() == RealMacroFile); }

		/*commun functions*/
		int cols_get();
		int rows_get();
		int size_get();

		std::string DimToString();

		virtual GenericType*	get(int _iPos){return NULL;};

    bool isIdentity(void);
    virtual bool isAssignable(void) { return true; }

		virtual RealType getType(void) { return RealGeneric; }
  protected :
  };


  /*
  ** List of types
  */
  typedef std::vector<InternalType *> typed_list;

}
#endif /* !__TYPES_HH__ */
