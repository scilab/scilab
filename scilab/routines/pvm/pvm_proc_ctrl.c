  
/* Copyright (c) 1997 by LIP ENS-LYON.  All Rights Reserved */

/***
   NAME
     pvm_proc_ctrl
   PURPOSE
     PVM Control process
   NOTES
     
   HISTORY
     fleury - Nov 6, 1997: Created.
     $Log: pvm_proc_ctrl.c,v $
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
#if defined(__EDG__)
#include <time.h>
#endif
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pvm3.h"
#include "../machine.h"
#include "sci_pvm.h"

#ifndef WIN32 
#include <unistd.h>
#endif 


#ifdef WIN32 
#ifndef __ABSC__
#include <sys/utime.h>
#else
#include <time.h>
#endif
#else 
#include <sys/time.h>
#endif /** WIN32 **/

#ifdef __ABSC__ /* For the definition of _stricmp */
#include <ctype.h>


int _stricmp(const char *s1, const char *s2)
{
  while (tolower(*s1) == tolower(*s2))
  {
    if (*s1 == 0)
      return 0;
    s1++;
    s2++;
  }
  return (int)tolower(*s1) - (int)tolower(*s2);
}
#endif

/* stat function */
#if (defined __MSC__) || (defined __ABSC__) || defined(__MINGW32__) 
#define stat _stat 
#endif 

#ifdef WIN32
extern void sciprint_nd (char *fmt,...);
extern void sciprint (char *fmt,...);
extern int pvmendtask();
#endif



typedef char *strings;

static strings Scipvm_error[]= { 
      "OK",
      "bad parameter",
      "Barrier count mismatch",
      "read past end of buffer",
      "no such host",
      "No such executable",
      "can not get memory",
      "can not decode received msg",
      "daemond pvmd is not responding",
      "no current buffer",
      "bad message id",
      "null group name is illegal",
      "already in group",
      "no group with that name",
      "not in group",
      "no such instance in group",
      "host failed",
      "no parent task",
      "function not implemented",
      "pvmd system error",
      "pvmd-pvmd protocol mismatch",
      "out of ressources",
      "host already configured",
      "failed to exec new slave pvmd",
      "already oing operation",
      "no such task",
      "no such (group,instance)",
      "(group,instance) already exists",
      "Unknow error",
};

/*--------------------------------------------------
 * get error message 
 *--------------------------------------------------*/

char *scipvm_error_msg(int err)
{
  switch (err)
    {
    case PvmOk:          return Scipvm_error[0];     break;
    case PvmBadParam:    return Scipvm_error[1];     break;
    case PvmMismatch:    return Scipvm_error[2];     break;
    case PvmNoData:      return Scipvm_error[3];     break;
    case PvmNoHost:      return Scipvm_error[4];     break;
    case PvmNoFile:      return Scipvm_error[5];     break;
    case PvmNoMem:       return Scipvm_error[6];     break;
    case PvmBadMsg:      return Scipvm_error[7];     break;
    case PvmSysErr:      return Scipvm_error[8];     break;
    case PvmNoBuf:       return Scipvm_error[9];     break;
    case PvmNoSuchBuf:   return Scipvm_error[10];     break;
    case PvmNullGroup:   return Scipvm_error[11];     break;
    case PvmDupGroup:    return Scipvm_error[12];     break;
    case PvmNoGroup:     return Scipvm_error[13];     break;
    case PvmNotInGroup:  return Scipvm_error[14];     break;
    case PvmNoInst:      return Scipvm_error[15];     break;
    case PvmHostFail:    return Scipvm_error[16];     break;
    case PvmNoParent:    return Scipvm_error[17];     break;
    case PvmNotImpl:     return Scipvm_error[18];     break;
    case PvmDSysErr:     return Scipvm_error[19];     break;
    case PvmBadVersion:  return Scipvm_error[20];     break;
    case PvmOutOfRes:    return Scipvm_error[21];     break;
    case PvmDupHost:     return Scipvm_error[22];     break;
    case PvmCantStart:   return Scipvm_error[23];     break;
    case PvmAlready:     return Scipvm_error[24];     break;
    case PvmNoTask:      return Scipvm_error[25];     break;
#ifdef PVM_MAJOR_VERSION 
    case PvmNotFound:    return Scipvm_error[26];     break;
    case PvmExists:      return Scipvm_error[27];     break;
#else
    case PvmNoEntry:     return Scipvm_error[26];     break;
    case PvmDupEntry:    return Scipvm_error[27];     break;
#endif
    default:             return Scipvm_error[28]; break;
    }
}



/*------------------------------------------------------------------------
 * start pvm 
 *------------------------------------------------------------------------*/


void C2F(scipvmstart)(int *res, char *hostfile, int *l)
{
  struct stat buf;
  char *rd = 0;
  char *ro = 0;
  char *path = NULL;
  int argc = 0;
  char *argv[2];

  argv[0] = "";
  argv[1] = (char*)0;
  if (!strcmp(hostfile, "null")) 
    {
	  /* on ne spécifie pas de hostfile */
      /* on essaye de prendre
	   * $HOME/.pvmd.conf, puis
	   * $SCI/.pvmd.conf sinon on laisse
	   *	 faire pvmd...
	   */ 
      if (!argc && (ro = getenv("PVM_ROOT")) && (rd = getenv("HOME"))){
		if ((path = (char *) malloc(strlen(rd)+12)) == NULL) {
		  (void) fprintf(stderr, "Error malloc in pvm_error\n");
		  *res = PvmNoMem;
		  return;
		}
		strcpy(path, rd);
		strcat(path, "/.pvmd.conf"); 
		if (stat(path, &buf) == 0){
		  argc = 1;
		  argv[0] = path;
		  sciprint_nd("The configuration file\n %s\nis used.\n", path);
		} else {
		  sciprint_nd("Warning: PVM_ROOT is set to %s\r\n",ro);
		  sciprint_nd("\tbut there exists no configuration file:\r\n");
		  sciprint_nd("\t%s\r\n", path);
		  free(path);
		}
      } /* PVM_ROOT + HOME */
      if (!argc && (rd = getenv("SCI"))){
		if ((path = (char *) malloc(strlen(rd)+12)) == NULL) {
		  (void) fprintf(stderr, "Error malloc in pvm_error\n");
		  *res = PvmNoMem;
		  return;
		}
		strcpy(path, rd);
		strcat(path, "/.pvmd.conf"); 
		if (stat(path, &buf) == 0){
	  	  sciprint_nd("The standard configuration file $SCI/.pvmd.conf will be used.\r\n");
		  sciprint_nd("\tWith SCI=%s\r\n",rd);
		  sciprint_nd("\tSCI will have to be set on remote hosts \r\n");
		  sciprint_nd("\tin order to spawn scilab\r\n",rd);
		  argc = 1;
		  argv[0] = path;
		} else {
		  free(path);
		  sciprint_nd("Warning: The standard configuration file $SCI/.pvmd.conf was not found.\r\n");
		  sciprint_nd("\tWe supposed that PVM and scilab are in standard place on your net\r\n");
		  sciprint_nd("\t (Cf. man pvmd3)\r\n");
		}
      } /* SCI */
    } else {
      if (stat(hostfile, &buf) == -1){
		sciprint("%s: No such file or directory\r\n", hostfile);
      } else {
	argv[0] = hostfile;
	argc = 1;
      }
    }
  *res = pvm_start_pvmd(argc, argv, 1);
} 



/*------------------------------------------------------------------------
 * halt pvm 
 *------------------------------------------------------------------------*/

void C2F(scipvmhalt)(int *res)
{
  /* Catch the SIGTERM */
  if (SIG_ERR == signal(SIGTERM,SIG_IGN)){
    (void) fprintf(stderr, "Error pvm_halt - signal\n");
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
    (void) fprintf(stderr, "Error pvm_halt - signal\n");
    *res = -1;
    return;
  }
#endif 
  if (SIG_ERR == signal(SIGTERM,SIG_DFL)){
    (void) fprintf(stderr, "Error pvm_halt - signal\n");
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

void C2F(scipvmspawn)(char *task,  int *l1, 
		      char *win,   int *l2,
		      char *where, int *l3, 
		      int *ntask,  int *tids, int *res)
{
  
  int flag = PvmTaskDefault;
  char cmd[256];
  char *arg[4];
  int nargs= -1;
    
  arg[0] = NULL;
   
  cmd[0] = 0;

  if ( !strcmp(where, "null") )
    where = NULL;
  else
    flag = PvmTaskHost;
#if (defined __MSC__) || (defined __ABSC__) || defined(__MINGW32__) 
  strcpy(cmd, "scilex.exe");
#else
  /* I really need scilab here for gtk -version */
  strcpy(cmd, "scilab");
#endif 
#if (defined __MSC__) || (defined __ABSC__) || defined(__MINGW32__) 
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
  sciprint("cmd=[%s]\r\n",cmd) ;
  nargs = 0; 
  for ( i = 0 ; arg[i] != NULL ; i++) 
    sciprint("arg %d =[%s]\r\n",i,arg[i]) ;
  */
  *res = pvm_spawn(cmd, arg, flag, where, *ntask, tids);
}

/*------------------------------------------------------------------------
 * spawn 
 *------------------------------------------------------------------------*/
  
void C2F(scipvmspawnindependent)(char *task,  int *l1, 
				 int *ntask,
				 char *where, int *l3, 
				 int *tids, int *res)
{
  int flag = PvmTaskDefault;

  if (!strcmp(where,"null"))
    where = NULL;
  else
    flag = PvmTaskHost;

  *res = pvm_spawn(task, NULL, flag, where, *ntask, tids);
} 




