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

	sciprint(_("                 Consortium Scilab (DIGITEO)\n"));

	sciprint(_("               Copyright (c) 1989-2009 (INRIA)\n"));
	sciprint(_("               Copyright (c) 1989-2007 (ENPC)\n"));
	sciprint("%s\n",line);

	#if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
		sciprint("\n\n");
		sciprint(_("Warning: the operational team of the Scilab Consortium\ndoesn't provide and doesn't support this version of Scilab built with\n"));
		sciprint("   ");
  #endif
}
/*--------------------------------------------------------------------------*/
int C2F(banier)(int *flag)
{
	if (*flag != 999)
	{
		banner();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/


