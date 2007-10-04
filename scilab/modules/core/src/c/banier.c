/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
#include "machine.h"
#include "version.h"
#include "sciprint.h"
#include "banier.h"
#include "message_scilab.h"
/*-----------------------------------------------------------------------------------*/
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
	message_scilab("                 Copyright (c) 1989-2007                   ");
	message_scilab("              Consortium Scilab (INRIA, ENPC)              ");
	sciprint("%s\n",line);

	sciprint("\n\n");
	sciprint("!!! WARNING !!!\n");
	sciprint("This software is being provided \"as is\", without any express or \n");
	sciprint("implied warranty.\n");
	sciprint("This is a development and not at all a final or stable version of Scilab 5.0\n");
	sciprint("that will be released in the future. A lot of features are missing and buggy.\n");
	sciprint("In no event will the authors be held liable for any damages arising from,\n");
	sciprint("out of or in connection with the software or the use or other dealings\n");
	sciprint("in the software.\n");

	#if defined(__APPLE__)
		sciprint("\n\n");
		sciprint("Warning: the operational team of the Scilab Consortium\n");
		sciprint("   doesn't provide and doesn't support the port of Scilab to MacOS.\n\n");
	#endif /* __APPLE__ */

	#if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
		sciprint("\n\n");
		sciprint("Warning: the operational team of the Scilab Consortium\n");
		sciprint("   doesn't provide and doesn't support this version of Scilab built with\n");
		sciprint("   ");
  #endif
}
/*-----------------------------------------------------------------------------------*/
int C2F(banier)(integer *flag)
{
	if (*flag != 999)
	{
		banner();
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/


