/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
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
#ifndef __NOMORESCILABMEMORYEXCEPTION__
#define __NOMORESCILABMEMORYEXCEPTION__
#include <exception>
#include "noMoreMemory.h"

namespace org_scilab_modules_external_objects_java
{

/**
 * Parent class for exceptions which may occure in JNI code.
 */
class NoMoreScilabMemoryException : public std::exception
{

public:

    NoMoreScilabMemoryException(void) throw() ;

    ~NoMoreScilabMemoryException(void) throw();

    /**
     * @return a description of the exception
     */
    const char * what(void) const throw();
};
}
#endif
/*--------------------------------------------------------------------------*/
