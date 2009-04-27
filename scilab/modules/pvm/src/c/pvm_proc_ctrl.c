/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * Copyright (C) 2002-2003 - INRIA - Serge STEER
 * Copyright (C) 2002-2005 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2004-2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

  
/* Copyright (c) 1997 by LIP ENS-LYON.  All Rights Reserved */

/***
   NAME
     pvm_proc_ctrl
   PURPOSE
     PVM Control process
   NOTES
     
   HISTORY
     fleury - Nov 6, 1997: Created.
     Revision 1.20  2005/10/22 18:53:10  cornet
     update memory management under Windows
     use HeapAlloc and VirtualAlloc (for scilab stack)
     Correction Bug 1576
     n=10000
     xbasc();
     plot2d([0,1],[0,n],0)
     xpols=[zeros(1,n); ones(2,n); zeros(1,n)];
     ypols=[2:n+1; 2:n+1; 1:n; 1:n];
     xfpolys(xpols,ypols,modulo((1:n),32))

     and for windows on PC with 256 mo
     stacksize(5000000*20)
     a=rand(9999,9999)

     Revision 1.19  2005/08/01 11:30:37  cornet
     Remove ABSOFT reference

     Revision 1.18  2005/07/03 18:16:15  cornet
     optimisation des MALLOC pour Windows ( A tester avec attention ) --> VirtualAlloc

     Revision 1.17  2005/07/01 07:08:13  cornet
     replace malloc, free, calloc & realloc by MALLOC,FREE,CALLOC & REALLOC defined in SCI/libs/MALLOC/includes/sci_mem_alloc.h

     Revision 1.16  2005/06/01 09:18:36  chancelier
     error messages

     Revision 1.15  2005/06/01 09:17:24  chancelier
     arguments returned by reduce are not the proper ones

     Revision 1.14  2005/05/27 13:52:29  chancelier
     headers added for Unix

     Revision 1.13  2005/01/07 20:49:25  cornet
     update for compilation via makefiles (windows)

     Revision 1.12  2004/02/28 13:50:09  cornet
     Corrections Warnings

     Revision 1.11  2003/11/04 09:36:06  steer
     back to previous version

     Revision 1.9  2003/10/22 09:03:27  steer
     ifdef added for intel compiler

     Revision 1.8  2002/09/12 12:24:47  chanceli
     updates

     Revision 1.7  2002/07/31 11:35:08  chanceli
     scilex-> scilab for gtk

     Revision 1.6  2002/07/31 06:58:39  chanceli
     changes to spawn scilab and not scilex + fprintf -> sciprint

     Revision 1.5  2002/07/29 14:37:49  chanceli
     vc++

     Revision 1.4  2002/07/25 08:42:44  chanceli
     updates

     Revision 1.3  2002/03/18 12:15:17  steer
     modif Jean-Sebastien Roy pour FreeBSD

     Revision 1.2  2002/02/08 10:27:33  steer
      modifs de mcmahill@mtl.mit.edu

     Revision 1.1.1.1  2001/04/26 07:49:01  scilab
     Imported sources

     Revision 1.16  1999/07/08 21:43:13  fleury
     Modifs relative a pvm_start([hostfile])

     Revision 1.11  1998/04/06 15:29:13  fleury
     ajout de pvm_error

     Revision 1.10  1998/04/03 11:40:59  fleury
     Ajout des commandes pvm_start et pvm_halt
     Correction d'erreurs en version non ANSI
     teste sur alpha et sun
     PBM de link en dynamique sur hpux9

     Revision 1.9  1998/03/27 12:20:12  fleury
     Version pvm OK.
     TODO: faire des tests de compil sur plateforme separee (POPC0
     TODO: commenter source (-;
     TODO: faire un peu de netoyage

     Revision 1.8  1998/03/26 12:33:01  fleury
     Version OK.
     TODO: faire un clean des .c
     TODO: enlever les _ ds les noms de fonctions (f77 compliant)
     TODO: faire des tests et un jeux de tests

     Revision 1.7  1998/03/13 13:57:03  fleury
     Version send/recv avec pack. A tester.
     TODO: ajouter les listes + BROADCAST
     TODO: faire un clean du dir et des fichiers...

     Revision 1.6  1997/12/01 11:08:05  fleury
     Spawn march een version reduite. Ajout de fct
     ds info et control

     Revision 1.5  1997/11/27 14:27:24  fleury
     Version de spawn pas encore au point tant que l on a pas
     scilex -nw -f <script>

     Revision 1.4  1997/11/26 09:51:27  fleury
     Nettoyage des code. LA version marche avec PVM.
     TODO: mettre le spawn de facon elegante !

     Revision 1.3  1997/11/25 11:15:52  fleury
     Ajout des command pvm pour la gestion des groupes.
     Tjs le pbm de TID multiples

     Revision 1.2  1997/11/21 13:59:49  fleury
     Ajout de nouveau fichier pour l'interface Scilab PVM
     modification du Makefile et des variables d'environnement

     Revision 1.1  1997/11/10 15:23:48  fleury
     Premier commit pour les control process PVM

***/

#include "MALLOC.h"

#ifdef __EDG__
#include <time.h>
#endif
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _MSC_VER 
#include <unistd.h>
#endif 

#ifdef _MSC_VER 
#include <sys/utime.h>
#else 
#include <sys/time.h>
#endif /** _MSC_VER **/

#include <stdarg.h>

/* stat function */
#ifdef _MSC_VER
#define stat _stat 
#endif 

#include "sci_pvm.h"
#include "setgetSCIpath.h"
#include "GetenvB.h"
#include "PATH_MAX.h"
#include "sciprint.h"
#include "sciprint.h"
#include "localization.h"

extern int pvmendtask(void);

/*--------------------------------------------------
 * get error message 
 *--------------------------------------------------*/


extern char *pvm_errlist[];
extern int pvm_nerr;

char *
pvm_geterror(int n)
{
  return (n <= 0 && n > -pvm_nerr ? pvm_errlist[-n] : _("Unknown Error"));
}

char *scipvm_error_msg(int err)
{
  switch (err)
    {
    case PvmOk:          return _("OK");     break;
    case PvmBadParam:    return _("Bad parameter");     break;
    case PvmMismatch:    return _("Barrier count mismatch");     break;
    case PvmNoData:      return _("Read past end of buffer");     break;
    case PvmNoHost:      return _("No such host");     break;
    case PvmNoFile:      return _("No such executable");     break;
    case PvmNoMem:       return _("Can not get memory");     break;
    case PvmBadMsg:      return _("Can not decode received msg");     break;
    case PvmSysErr:      return _("Daemon pvmd is not responding");     break;
    case PvmNoBuf:       return _("No current buffer");     break;
    case PvmNoSuchBuf:   return _("Bad message id");     break;
    case PvmNullGroup:   return _("Null group name is illegal");     break;
    case PvmDupGroup:    return _("Already in group");     break;
    case PvmNoGroup:     return _("No group with that name");     break;
    case PvmNotInGroup:  return _("Not in group");     break;
    case PvmNoInst:      return _("No such instance in group");     break;
    case PvmHostFail:    return _("Host failed");     break;
    case PvmNoParent:    return _("No parent task");     break;
    case PvmNotImpl:     return _("Function not implemented");     break;
    case PvmDSysErr:     return _("Pvmd system error");     break;
    case PvmBadVersion:  return _("Pvmd-pvmd protocol mismatch");     break;
    case PvmOutOfRes:    return _("Out of ressources");     break;
    case PvmDupHost:     return _("Host already configured (Duplicate host)");     break;
    case PvmCantStart:   return _("Failed to exec new slave pvmd");     break;
    case PvmAlready:     return _("Already oing operation");     break;
    case PvmNoTask:      return _("No such task");     break;
#ifdef PVM_MAJOR_VERSION 
    case PvmNotFound:    return _("PVM not found");     break;
    case PvmExists:      return _("PVM exists");     break;
#else
#ifdef PvmNoEntry
    case PvmNoEntry:     return _("No such (group,instance)");     break;
#endif

#ifdef PvmDupEntry
    case PvmDupEntry:    return _("(group,instance) already exists");     break;
#endif
#endif
    default:             return _("Unknown Error"); break;
    }
}



/*------------------------------------------------------------------------
 * start pvm 
 *------------------------------------------------------------------------*/

void C2F(scipvmstart)(int *res, char *hostfile, int *hostfile_len)
{
	struct stat buf;
	char *rd = 0;
	char *ro = 0;
	char *path = NULL;
	int argc = 0;
	char *argv[2];
	int block=1;

	argv[0] = "";
	argv[1] = (char*)0;
	if (!strcmp(hostfile, "null")) 
	{
		/* If hostfile is not specified, we try
		 * $HOME/.pvmd.conf, then
		 * $SCI/.pvmd.conf 
	     * if both files are not found, let pvmd does his work
		 */ 
		static char HOME[PATH_MAX];
		int nc = PATH_MAX;
		GetenvB("HOME"  , HOME,nc);

		if (!argc && (nc))
		{
			if ((path = (char *) MALLOC( (nc+strlen(PVM_CONFIG_FILE)+1) *sizeof(char))) == NULL) 
			{
				(void) fprintf(stderr, "%s",_("Error MALLOC in pvm_error\n"));
				*res = PvmNoMem;
				return;
			}
			strcpy(path, HOME);
			strcat(path, PVM_CONFIG_FILE); 
			if (stat(path, &buf) == 0)
			{
				argc = 1;
				argv[0] = path;
				sciprint(_("The configuration file %s is used.\n"), path);
			} 
			else 
			{
				ro = getenv("PVM_ROOT");
				if (ro != NULL)
				{
					sciprint(_("Warning: PVM_ROOT is set to %s\n"),ro);
					sciprint(_("but there exists no configuration file:\n"));
					sciprint("%s\n", path);
					FREE(path);
				}
				else
				{
					sciprint(_("Warning: PVM_ROOT is not set.\n"),ro);
				}
			}
		}
		/* PVM_ROOT + HOME */
		if (!argc && (rd = getSCIpath()))
		{
			if ((path = (char *) MALLOC(strlen(rd)+strlen(PVM_MODULE)+strlen(PVM_CONFIG_FILE)+1)) == NULL) 
			{
				(void) fprintf(stderr, _("%s: No more memory.\n"),"pvm_start");
				*res = PvmNoMem;
				return;
			}
			strcpy(path, rd);
			strcat(path, PVM_MODULE); 
			strcat(path, PVM_CONFIG_FILE); 
			if (stat(path, &buf) == 0)
			{
				sciprint(_("The standard configuration file $SCI%s will be used.\nWith SCI=%s\nSCI will have to be set on remote hosts\nin order to spawn scilab\n"),PVM_CONFIG_FILE,rd,rd);
				/* Standard Scilab configuration file needs env variables PVM_ROOT, SCI, PVM_ARCH */
				/* If they are not set then return */
				if (getenv("PVM_ROOT") == NULL || getenv("PVM_ARCH") == NULL || getenv("SCI"))
				{
					sciprint(_("The standard configuration file $SCI%s%s needs the environment variables %s, %s, %s to be set. PVM can not be started.\n"),PVM_MODULE, PVM_CONFIG_FILE, "PVM_ROOT", "PVM_ARCH", "SCI");
					*res = -1;
					return;
				}
				argc = 1;
				argv[0] = path;
			} 
			else
			{
				FREE(path);
				sciprint(_("Warning: The standard configuration file $SCI%s was not found.\nWe supposed that PVM and scilab are in standard place on your net\n (Cf. man pvmd3)\n"),PVM_CONFIG_FILE);
			}
		} /* SCI */
	}
	else 
	{
		if (stat(hostfile, &buf) == -1)
		{
			sciprint(_("%s: No such file or directory.\n"), hostfile);
		} 
		else 
		{
			argv[0] = hostfile;
			argc = 1;
		}
	}
	*res = pvm_start_pvmd(argc, argv, block);
} 



/*------------------------------------------------------------------------
 * halt pvm 
 *------------------------------------------------------------------------*/

void C2F(scipvmhalt)(int *res)
{
  /* Catch the SIGTERM */
  if (SIG_ERR == signal(SIGTERM,SIG_IGN)){
    (void) fprintf(stderr, "%s", _("Error pvm_halt - signal\n"));
    *res = -1;
    return;
  }
  *res =  pvm_halt(); 
  if (*res == 0) {
    *res = pvmendtask();
  }
  /* Catch the SIGPIPE and deflect SIGTERM */
#ifdef SIGPIPE
  if (SIG_ERR == signal(SIGPIPE,SIG_IGN)){
    (void) fprintf(stderr, "%s", _("Error pvm_halt - signal\n"));
    *res = -1;
    return;
  }
#endif 
  if (SIG_ERR == signal(SIGTERM,SIG_DFL)){
    (void) fprintf(stderr, "%s", _("Error pvm_halt - signal\n"));
    *res = -1;
    return;
  }         
} 


/*------------------------------------------------------------------------
 * timer 
 *------------------------------------------------------------------------*/

static struct timeval t1;

void C2F(scipvmsettimer)(int *res)
{
  *res = gettimeofday(&t1, NULL);
} 

void C2F(scipvmgettimer)(double *res)
{
  struct timeval t2;
  *res = gettimeofday(&t2, NULL);
  if (*res != -1){
    *res = (double)(t2.tv_sec-t1.tv_sec)*1000000.+(double)(t2.tv_usec-t1.tv_usec);
    t1 = t2;
  }
} 


/*------------------------------------------------------------------------
 * spawn a scilab 
 *------------------------------------------------------------------------*/

void C2F(scipvmspawn)(char *task,  int *l1, char *win,   int *l2, char *where, int *l3, int *ntask,  int *taskId, int *res)
{
  
  int flag = PvmTaskDefault;
  char cmd[256];
  char *arg[4];
  int nargs= -1;
    
  arg[0] = NULL;
   
  cmd[0] = 0;

  if ( !strcmp(where, "null") )
	  {
    where = NULL;
	  }
  else
	  {
    flag = PvmTaskHost;
	  }
  /* TODO : make a better detection of the remote scilab bin */
#ifdef _MSC_VER
  strcpy(cmd, "scilex.exe");
#else
  strcpy(cmd, "scilab");
#endif 
#ifdef _MSC_VER
  if ( _stricmp(task,"null") != 0) 
#else 
  if (strcasecmp(task, "null")) 
#endif
    {
      arg[++nargs] = "-f";
      arg[++nargs] = task;
    }
  
  if ( strcmp(win, "nw") == 0 )  arg[++nargs] = "-nw";
  arg[++nargs]=NULL;
  /* 
  sciprint("cmd=[%s]\n",cmd) ;
  nargs = 0;
  int i = 0;
  for (i = 0 ; arg[i] != NULL ; i++) 
    sciprint("arg %d =[%s]\n",i,arg[i]) ;
  sciprint("flag = %d\n", flag);
  sciprint("where = %s\n", where);
  sciprint("*ntask = %d\n", *ntask);
  */
  *res = pvm_spawn(cmd, arg, flag, where, *ntask, taskId);
  /* res: A positive value less than ntask indicates a partial failure. In this case the user should check the tids array for the error code(s). */
  if (*res < *ntask)
    {
      (void) fprintf(stderr,_("%s: Error while creating processes: %s.\n"), "scipvmspawn", scipvm_error_msg(taskId[0]));
    }
}

/*------------------------------------------------------------------------
 * spawn 
 *------------------------------------------------------------------------*/
  
void C2F(scipvmspawnindependent)(char *task,  int *l1, int *ntask, char *where, int *l3, int *taskId, int *res)
{
	int flag = PvmTaskDefault;
	char **argv=NULL;

  if (!strcmp(where,"null"))
    where = NULL;
  else
    flag = PvmTaskHost;

  *res = pvm_spawn(task, argv, flag, where, *ntask, taskId);
} 




