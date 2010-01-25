/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Several C++ functions that parform some basic operations
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <exception>
#include <iostream>

#include "rendererBasicAlgos.h"
extern "C"
{
#include "sciprint.h"
#include "localization.h"
#include "DestroyObjects.h"
}

/*--------------------------------------------------------------------------*/
void destroyRendererString(char * string)
{
  delete[] string;
}
/*--------------------------------------------------------------------------*/
/* It must be moved in SCI_GATEWAY directory */
/* NOT in renderer in GRAPHICS */
void callFunctionFromGatewayWithExceptions(gw_generic_table * tab, int sizetab)
{
	try
	{
		// Exception might be generated in the renderer module.
		callFunctionFromGateway(tab,sizetab);
	}
	catch (std::exception & e)
	{
		// Tell the user that somthing wrong occured
		sciprint(const_cast<char*>(_("Warning !!!\nScilab has found a critical error (%s).\nSave your data and restart Scilab.\n")), "Unknown exception");
		// print the exception in the error output
		std::cerr << e.what() << std::endl;
	}
}
/*--------------------------------------------------------------------------*/
