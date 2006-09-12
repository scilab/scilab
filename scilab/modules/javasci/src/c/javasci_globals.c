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
  static char initstr[]="exec(\"SCI/etc/scilab.start\",-1);quit;";
  static int iflag=-1, stacksize = 1000000, ierr=0;

  #if _MSC_VER
    static char JavaSciInterf[]="javasci";
    static char nw[]="-nw";
    static char nb[]="-nb";
  #endif

  
  char *p1 = (char*)getenv ("SCI");
  
  
  #if _MSC_VER
  /* Supprime le mode windows et la baniere */
    add_sci_argv(JavaSciInterf);
    add_sci_argv(nb);
  #endif
  
  #if _MSC_VER 
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
   if (p1==NULL)
   {
   	fprintf(stderr,"Please define SCI environment variable\n");
   	sprintf (env, "%s=%s", "SCI",SCI);
		putenv (env);
   }
  #endif
  /* set TMPDIR */
  C2F(settmpdir)();
  /* Scilab Initialization */
  C2F(inisci)(&iflag,&stacksize,&ierr);
  if ( ierr > 0 ) 
    {
      fprintf(stderr,"Scilab initialization failed !\n");
      exit(1);
    }


  /* Initialisation fenetre graphique */
  #if _MSC_VER
    InitWindowGraphDll();
  #endif

  /* pour initialisation de la primitive scilab : fromjava() */
  SetFromJavaToON();

  /* Chargement de Scilab.start */
  C2F(scirun)(initstr,(int)strlen(initstr));
 
}
/********************************************************************************************************/
int send_scilab_job(char *job) 
{
  return (int) SendScilabJob(job); 
}
/********************************************************************************************************/
#ifndef _MSC_VER
int MAIN__() 
{
	return 0;
}
#endif
/********************************************************************************************************/
