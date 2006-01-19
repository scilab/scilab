#include "javasci_globals.h"
/********************************************************************************************************/
/* Allan CORNET */
/* INRIA 2005 */
/********************************************************************************************************/
static int init = 0;
/********************************************************************************************************/
void EnableInterf()
{
	init = 1;
}
/********************************************************************************************************/
void DisableInterf()
{
	init = 0;
}
/********************************************************************************************************/
int GetInterfState()
{
	return init;
}
/********************************************************************************************************/
/*   Initialisation de Scilab */
/********************************************************************************************************/
void Initialize() 
{
  static char env[1024];
  static char initstr[]="exec(\"SCI/scilab.star\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;

  #if WIN32
	static char JavaSciInterf[]="javasci";
    static char nw[]="-nw";
    static char nb[]="-nb";
  #endif

  #if WIN32
    char *p1 = (char*)getenv ("SCI");
  #endif
  
  #if WIN32
  /* Supprime le mode windows et la baniere */
	add_sci_argv(JavaSciInterf);
    add_sci_argv(nb);
  #endif
  
  #if WIN32 
    if ( p1== NULL )
    {
		/* Detection Scilab path */
		char modname[MAX_PATH+1];
		if (!GetModuleFileName (GetModuleHandle("javasci.dll"), modname, MAX_PATH))
		{
			MessageBox(NULL,"javasci.dll not found","Warning",MB_ICONWARNING);
		}
		else
		{
			char *p;
			if ((p = strrchr (modname, '\\')) == NULL) exit(1); /* remove \javasci.dll from modname */
			else
			{
				*p='\0';
				if ((p = strrchr (modname, '\\')) == NULL) exit(1); /* remove \bin from modname */
				else
				{
					*p='\0';
					set_sci_env(modname);
				}
			}
		}
    }
    else 
	{
		set_sci_env(p1);
	}
  #else
   sprintf (env, "%s=%s", "SCI",SCI);
   putenv (env);
  #endif

  /* Scilab Initialization */
  initTCLTK(); /* TCLTK Init. */
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }
  /* running the startup */ 
  C2F(settmpdir)();

  /* Initialisation fenetre graphique */
  #if WIN32
    InitWindowGraphDll();
    start_sci_gtk() ;
  #endif

  /* pour initialisation de la primitive scilab : fromjava() */
  SetFromJavaToON();

  /* Chargement de Scilab.star */
  C2F(scirun)(initstr,(int)strlen(initstr));
 
}
/********************************************************************************************************/
int send_scilab_job(char *job) 
{
  return (int) SendScilabJob(job); 
}
/********************************************************************************************************/
int MAIN__() 
{
	return 0;
}
/********************************************************************************************************/
