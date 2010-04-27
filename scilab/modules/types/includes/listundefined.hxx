/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __LISTUNDEFINED_HXX__
#define __LISTUNDEFINED_HXX__

#include <list>
#include "types.hxx"
#include "listoperation.hxx"

namespace types
{
	class ListUndefined : public ListOperation
  {
  public :
    ListUndefined(): ListOperation(){};
    virtual ~ListUndefined();

    // FIXME : Should not return NULL;
    ListUndefined* clone();

    ListUndefined *getAsListUndefined(void);
    RealType getType(void);
    
    void whoAmI();

    std::string toString(int _iPrecision, int _iLineLen);
  
    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::string         getTypeStr() {return string("listundefined");}
	};
}


#endif /* !__LISTUNDEFINED_HXX__ */
