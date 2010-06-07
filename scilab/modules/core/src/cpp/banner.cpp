/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "banner.hxx"
#include "yaspio.hxx"

extern "C" 
{
#include <string.h>
#include <math.h>
#include "version.h"
#include "localization.h"
}

void banner(void)
{
	int i;
	char *line = "        ___________________________________________        ";
	int startVersion = (int)(floor((double)(strlen(line)/2)) - floor((double)(strlen(SCI_VERSION_STRING)/2)));
	
   YaspWrite(line);
    YaspWrite("\n");

/* To center the version name */
    for( i=0 ; i<startVersion ; i++ )
    {
        YaspWrite(" ");
    }

    YaspWrite(SCI_VERSION_STRING);
    YaspWrite("\n\n");

    YaspWrite(_("                 Consortium Scilab (DIGITEO)\n"));

    YaspWrite(_("               Copyright (c) 1989-2009 (INRIA)\n"));
    YaspWrite(_("               Copyright (c) 1989-2007 (ENPC)\n"));
    YaspWrite(line);
    YaspWrite("\n");
    YaspWrite("\n");
    YaspWrite("           -*- THIS IS SCILAB 6.0 aka YaSp -*-\n");
    YaspWrite("\n");
    YaspWrite(line);
    YaspWrite("\n");

#if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
    YaspWrite("\n\n");
    YaspWrite(_("Warning: the operational team of the Scilab Consortium\ndoesn't provide and doesn't support this version of Scilab built with\n"));
    YaspWrite("   ");
#endif
}
