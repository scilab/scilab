/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
struct MODULESLIST
{
    char **ModuleList;
    int numberofModules;
};

/*--------------------------------------------------------------------------*/
/**
 * Get the list of modules
 *
 * @return List of modules
 */
struct MODULESLIST *getmodules(void);

/**
 * <long-description>
 *
 * @return the result of the operation
 */
BOOL DisposeModulesInfo(void);
/*--------------------------------------------------------------------------*/
