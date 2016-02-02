/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __EOFIELDSGETTER_HXX__
#define __EOFIELDSGETTER_HXX__

#include <string>
#include <cstring>
#include <vector>
#include "FieldsGetter.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
}

namespace org_modules_completion
{

/**
 * @file
 * @author Calixte DENIZET <calixte.denizet@scilab.org>
 *
 * Class to manage the retrievment of the fields name in objects wrapped by External Objects module
 */
class EOFieldsGetter : FieldsGetter
{

public :

    virtual const char ** getFieldsName(const std::string & typeName, int * mlist, char ** fieldPath, const int fieldPathLen, int * fieldsSize) const;

    /**
     * Initialization
     */
    static void initializeEO()
    {
        FieldsManager::addFieldsGetter(std::string("_EClass"), new EOFieldsGetter());
        FieldsManager::addFieldsGetter(std::string("_EObj"), new EOFieldsGetter());
    }

private :

    EOFieldsGetter() { }

    virtual ~EOFieldsGetter() { }
};
}

#endif
