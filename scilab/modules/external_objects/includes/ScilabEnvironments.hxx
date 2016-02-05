/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#ifndef __SCILABENVIRONMENTS_HXX__
#define __SCILABENVIRONMENTS_HXX__

//disable warnings about exports STL items
#pragma warning (disable : 4251)

#include <vector>

#include "ScilabAbstractEnvironmentException.hxx"
#include "ScilabAbstractEnvironment.hxx"
#include "dynlib_external_objects_scilab.h"

extern "C" {
#include "localization.h"
}

namespace org_modules_external_objects
{

class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabEnvironments
{
    static std::vector<ScilabAbstractEnvironment*> environments;

public:

    static int registerScilabEnvironment(ScilabAbstractEnvironment * env);

    static void unregisterScilabEnvironment(const int id);

    static ScilabAbstractEnvironment & getEnvironment(const int id);
};

}

#endif // __SCILABENVIRONMENTS_HXX__
