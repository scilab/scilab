/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#ifndef __SCILABABSTRACTOPTIONSSETTER_HXX__
#define __SCILABABSTRACTOPTIONSSETTER_HXX__

namespace org_modules_external_objects
{

class ScilabAbstractOptionsSetter
{

public:

    virtual void set(const bool state) = 0;

    virtual bool get() const = 0;

};
}

#endif //__SCILABABSTRACTOPTIONSSETTER_HXX__
