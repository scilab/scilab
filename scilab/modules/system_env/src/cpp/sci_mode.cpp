/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "sci_mode.h"

#include "configvariable.hxx"

scilabMode getScilabMode(void)
{
    return static_cast<scilabMode>(ConfigVariable::getScilabMode());
}

void setScilabMode(scilabMode newmode)
{
    ConfigVariable::setScilabMode(static_cast<int>(newmode));
}

char * getScilabModeString(void)
{
    scilabMode smMode = getScilabMode();
    switch(smMode)
    {
		case SCILAB_API:
			return "API";
			break;
		case SCILAB_STD:
			return "STD";
			break;
		case SCILAB_NW:
			return "NW";
			break;
		case SCILAB_NWNI:
			return "NWNI";
			break;
		default:
			return "STD";
			break;
	}
}
