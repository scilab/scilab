/* Copyright ENPC/Chancelier Jean-Philippe */

/********************************
 * Set up environment variables if 
 * necessary for WIN32 
 ********************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#if (defined _MSC_VER) 
#undef putenv
#define putenv(x) _putenv(x)
#endif

void SciEnv ()
{
  char *p,*p1;
  char modname[MAX_PATH+1];
  char env[MAX_PATH+1+10];
  if (!GetModuleFileName (NULL, modname+1, MAX_PATH))
    return;
  if ((p = strrchr (modname+1, '\\')) == NULL)
    return;
  *p = 0;

  /* Set SCI variable */
  if ((p = strrchr (modname+1, '\\')))
    {
      *p = 0;
      for (p = modname+1; *p; p++)
	{
	  if (*p == '\\') *p = '/';
	}
    p = modname + 1;
    if ( ( p1 = getenv("SCI"))  == (char *) 0 )
	{
		sprintf(env,"SCI=%s",p);
		#if _MSC_VER
			_putenv(env);
		#else
			putenv(env);
		#endif
	}
  }
}



