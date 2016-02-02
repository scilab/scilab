/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Igor GRIDCHYN
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
