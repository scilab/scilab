/*
 * PIMS ( http://forge.scilab.org/index.php/p/pims ) - This file is part of PIMS
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

#ifndef __JAVAOPTIONSSETTER_HXX__
#define __JAVAOPTIONSSETTER_HXX__

#include "ScilabOptionsSetter.hxx"
#include "dynlib_external_objects_java_scilab.h"

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{

class JavaOptionsHelper;

class EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP JavaOptionsSetter : public ScilabOptionsSetter
{

public:

    static const unsigned int SHOWPRIVATE;
    static const unsigned int ATTACHMODULE;

    JavaOptionsSetter(JavaOptionsHelper & _helper, unsigned int _type);
    ~JavaOptionsSetter() { }

    void set(const bool state);

    bool get() const;
};
}

#endif // __JAVAOPTIONSSETTER_HXX__
