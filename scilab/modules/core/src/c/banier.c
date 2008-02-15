/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <math.h>
#include <string.h>
#include "machine.h"
#include "version.h"
#include "sciprint.h"
#include "banier.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void banner(void)
{
	int i;
	char *line = "        ___________________________________________        ";
	int startVersion = (int)(floor((double)(strlen(line)/2)) - floor((double)(strlen(SCI_VERSION_STRING)/2)));
	
	sciprint("%s\n",line);
	
	/* To center the version name */
	for( i=0 ; i<startVersion ; i++ )
	{
		sciprint(" ");
	}
	
	sciprint("%s\n\n",SCI_VERSION_STRING);
	sciprint(_("                 Copyright (c) 1989-2008                   \n"));
	sciprint(_("              Consortium Scilab (INRIA, ENPC)              \n"));
	sciprint("%s\n",line);

	sciprint("\n\n");
	sciprint(_("!!! WARNING !!!\n"));
	sciprint(_("This software is being provided \"as is\", without any express or\nimplied warranty.\n"));
	sciprint(_("This is a development and not at all a final or stable version of Scilab 5.0\nthat will be released in the future. A lot of features are missing and buggy.\n"));
	sciprint(_("In no event will the authors be held liable for any damages arising from,\nout of or in connection with the software or the use or other dealings\nin the software.\n"));

	#if defined(__APPLE__)
		sciprint("\n\n");
		sciprint(_("Warning: the operational team of the Scilab Consortium\ndoesn't provide and doesn't support the port of Scilab to MacOS.\n"));
	#endif /* __APPLE__ */

	#if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
		sciprint("\n\n");
		sciprint(_("Warning: the operational team of the Scilab Consortium\ndoesn't provide and doesn't support this version of Scilab built with\n"));
		sciprint("   ");
  #endif
}
/*--------------------------------------------------------------------------*/
int C2F(banier)(integer *flag)
{
	if (*flag != 999)
	{
		banner();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/


