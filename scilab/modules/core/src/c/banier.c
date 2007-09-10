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
	
	sciprint("%s\r\n",line);
	
	/* To center the version name */
	for( i=0 ; i<startVersion ; i++ )
	{
		sciprint(" ");
	}
	
	sciprint("%s\r\n\n",SCI_VERSION_STRING);
	message_scilab("core_message_119");
	message_scilab("core_message_120");
	sciprint("%s\r\n",line);

	sciprint("\r\n\r\n");
	sciprint("!!! WARNING !!!\r\n");
	sciprint("This software is being provided \"as is\", without any express or \r\n");
	sciprint("implied warranty.\r\n");
	sciprint("This is a development and not at all a final or stable version of Scilab 5.0\r\n");
	sciprint("that will be released in the future. A lot of features are missing and buggy.\r\n");
	sciprint("In no event will the authors be held liable for any damages arising from,\r\n");
	sciprint("out of or in connection with the software or the use or other dealings\r\n");
	sciprint("in the software.\r\n");

	#if defined(__APPLE__)
		sciprint("\r\n\r\n");
		sciprint("Warning: the operational team of the Scilab Consortium\r\n");
		sciprint("   doesn't provide and doesn't support the port of Scilab to MacOS.\r\n\r\n");
	#endif /* __APPLE__ */

	#if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
		sciprint("\r\n\r\n");
		sciprint("Warning: the operational team of the Scilab Consortium\r\n");
		sciprint("   doesn't provide and doesn't support this version of Scilab built with\r\n");
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


