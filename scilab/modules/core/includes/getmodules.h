/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "machine.h"
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
