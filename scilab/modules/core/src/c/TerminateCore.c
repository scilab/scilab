/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
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
/*--------------------------------------------------------------------------*/ 
BOOL TerminateCorePart1(void)
{
	if ( Get_no_startup_flag() == 0) 
	{
		char *quit_script =  get_sci_data_strings(QUIT_ID);
		C2F(scirun)(quit_script,(long int)strlen(quit_script));
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
BOOL TerminateCorePart2(void)
{
	#ifdef _MSC_VER /* Bug sous Linux lors de la liberation memoire */
		C2F(freegmem)();
		C2F(freemem)();
	#endif

	DisposeModulesInfo();

	destroy_hashtable_scilab_functions();

	/* Close all scilab's files */
	TerminateScilabFilesList();

	/** clean tmpfiles **/
	C2F(tmpdirc)();

	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
