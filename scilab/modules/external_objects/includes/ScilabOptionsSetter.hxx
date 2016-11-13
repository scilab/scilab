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

/*--------------------------------------------------------------------------*/

#ifndef __SCILABOPTIONSSETTER_HXX__
#define __SCILABOPTIONSSETTER_HXX__

#include "ScilabAbstractOptionsSetter.hxx"
#include "dynlib_external_objects_scilab.h"

#define LAST_OPTION_INDEX 4

namespace org_modules_external_objects
{

class OptionsHelper;

class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabOptionsSetter : public ScilabAbstractOptionsSetter
{

protected:

    OptionsHelper & helper;
    unsigned int type;

public:

    static const unsigned int METHODOFCONV;
    static const unsigned int ALLOWRELOAD;
    static const unsigned int AUTOUNWRAP;
    static const unsigned int USECOPY;
    static const unsigned int USESCILABINDEX;

    ScilabOptionsSetter(OptionsHelper & _helper, unsigned int _type) : helper(_helper), type(_type) { }

    ScilabOptionsSetter(const ScilabOptionsSetter & setter) : helper(setter.helper), type(setter.type) { }

    ~ScilabOptionsSetter() { }

    virtual void set(const bool state);

    virtual bool get() const;
};
}

#endif //__SCILABOPTIONSSETTER_HXX__
