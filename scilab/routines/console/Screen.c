/* Modification Allan CORNET : 
 * J.Ph Chancelier 
 */

#include "../machine.h" 

#ifdef WITH_GTK
#ifdef WITH_READLINE 
/* this file is unused */ 
#define GTK_READLINE 
#endif 
#endif 

#ifndef GTK_READLINE  /* the gtk readline version is in gtk */ 
#ifndef WIN32 /** The win32 version is defined in the wsci directory **/

#include "../graphics/Graphics.h" /* for sciprint !! */

/* cls and tohome */

void HomeFunction(void)
{
  sciprint("\nNot yet implemented. \n");
}

void ClearScreenConsole(char *fname)
{
  sciprint("\nNot yet implemented. \n");
}

#endif
#endif 
