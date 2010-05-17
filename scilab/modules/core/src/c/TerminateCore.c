/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <libxml/parser.h>

#include "TerminateCore.h"
/*--------------------------------------------------------------------------*/ 
#include "realmain.h" /* Get_no_startup_flag */
#include "inffic.h" /* get_sci_data_strings */
#include "scirun.h" /* scirun */
#include "getmodules.h"
#include "scimem.h" /* freegmem */
#include "tmpdir.h" /* tmpdirc */
#include "hashtable_core.h" /* destroy_hashtable_scilab_functions */
#include "filesmanagement.h"
#include "scilabmode.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
BOOL TerminateCorePart1(void)
{
	if ( Get_no_startup_flag() == 0) 
	{
		char *quit_script = NULL;

		/* bug 3672 */
		if (getScilabMode() == SCILAB_STD) quit_script = get_sci_data_strings(QUIT_ERRCATCH_ID);
		else quit_script = get_sci_data_strings(QUIT_ID);

		/* launch scilab.quit script */
		C2F(scirun)(quit_script,(long int)strlen(quit_script));
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
BOOL TerminateCorePart2(void)
{
	#ifdef _MSC_VER /* Bug sous Linux lors de la liberation memoire */
		#ifndef _WIN64
		C2F(freegmem)();
		C2F(freemem)();
		#endif
	#endif

	DisposeModulesInfo();

	destroy_hashtable_scilab_functions();

	/* Close all scilab's files */
	TerminateScilabFilesList();

	/*
	 * Cleanup function for the XML library.
	 */
	xmlCleanupParser();

	/** clean tmpfiles **/
	C2F(tmpdirc)();

	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
