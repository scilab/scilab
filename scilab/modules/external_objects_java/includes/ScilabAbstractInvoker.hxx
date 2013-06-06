/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Igor GRIDCHYN
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABABSTRACTINVOKER_HXX__
#define __SCILABABSTRACTINVOKER_HXX__

namespace org_modules_external_objects
{
class ScilabAbstractInvoker
{

public:

    virtual int invoke(int * args, int argsSize) = 0;
};

}

#endif // __SCILABABSTRACTINVOKER_HXX__
