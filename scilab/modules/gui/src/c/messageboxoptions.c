/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT (java version)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "messageboxoptions.h"
#include "stricmp.h"

/*--------------------------------------------------------------------------*/

int isModalOption(char *option)
{
  return (!stricmp(option, "modal") || !stricmp(option, "non-modal"));
}

/*--------------------------------------------------------------------------*/

int isIconName(char *name)
{
  return (stricmp(name, "scilab")
          || stricmp(name, "error")
          || stricmp(name, "hourglass")
          || stricmp(name, "info")
          || stricmp(name, "passwd")
          || stricmp(name, "question")
          || stricmp(name, "warning"));
}

/*--------------------------------------------------------------------------*/
