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
     Revision 1.1  2001/04/26 07:49:01  scilab
     Initial revision

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
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pvm3.h"
#include "../machine.h"
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

int
_stricmp(const char *s1, const char *s2)
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


static struct timeval t1;

#ifdef __STDC__
void 
C2F(scipvmerror)(int *err, char ***res)
#else
void 
C2F(scipvmerror)(err, res)
  int *err;
  char ***res;
#endif 
{
  if (((*res) = (char**) malloc(sizeof(char**))) == NULL) {
    (void) fprintf(stderr, "Error malloc in pvm_error\n");
    return;
  }
  if ((**res = (char *) malloc(255 * sizeof(char))) == NULL) {
    (void) fprintf(stderr, "Error malloc in pvm_error\n");
    return;
  }
  switch (*err)
    {
    case PvmOk:
      (void) sprintf(**res, "%s", "OK");
      break;
    case PvmBadParam:
      (void) sprintf(**res, "%s", "bad parameter");
      break;
    case PvmMismatch:
      (void) sprintf(**res, "%s", "Barrier count mismatch");
      break;
    case PvmNoData:
      (void) sprintf(**res, "%s", "read past end of buffer");
      break;
    case PvmNoHost:
      (void) sprintf(**res, "%s", "no such host");
      break;
    case PvmNoFile:
      (void) sprintf(**res, "%s", "No such executable");
      break;
    case PvmNoMem:
      (void) sprintf(**res, "%s", "can not get memory");
      break;
    case PvmBadMsg:
      (void) sprintf(**res, "%s", "can not decode received msg");
      break;
    case PvmSysErr:
      (void) sprintf(**res, "%s", "daemond pvmd is not responding");
      break;
    case PvmNoBuf:
      (void) sprintf(**res, "%s", "no current buffer");
      break;
    case PvmNoSuchBuf:
      (void) sprintf(**res, "%s", "bad message id");
      break;
    case PvmNullGroup:
      (void) sprintf(**res, "%s", "null group name is illegal");
      break;
    case PvmDupGroup:
      (void) sprintf(**res, "%s", "already in group");
      break;
    case PvmNoGroup:
      (void) sprintf(**res, "%s", "no group with that name");
      break;
    case PvmNotInGroup:
      (void) sprintf(**res, "%s", "not in group");
      break;
    case PvmNoInst:
      (void) sprintf(**res, "%s", "no such instance in group");
      break;
    case PvmHostFail:
      (void) sprintf(**res, "%s", "host failed");
      break;
    case PvmNoParent:
      (void) sprintf(**res, "%s", "no parent task");
      break;
    case PvmNotImpl:
      (void) sprintf(**res, "%s", "function not implemented");
      break;
    case PvmDSysErr:
      (void) sprintf(**res, "%s", "pvmd system error");
      break;
    case PvmBadVersion:
      (void) sprintf(**res, "%s", "pvmd-pvmd protocol mismatch");
      break;
      case PvmOutOfRes:
      (void) sprintf(**res, "%s", "out of ressources");
      break;
    case PvmDupHost:
      (void) sprintf(**res, "%s", "host already configured");
      break;
    case PvmCantStart:
      (void) sprintf(**res, "%s", "failed to exec new slave pvmd");
      break;
    case PvmAlready:
      (void) sprintf(**res, "%s", "already oing operation");
      break;
    case PvmNoTask:
      (void) sprintf(**res, "%s", "no such task");
      break;
#ifdef PVM_MAJOR_VERSION 
    case PvmNotFound:
      (void) sprintf(**res, "%s", "no such (group,instance)");
      break;
    case PvmExists:
      (void) sprintf(**res, "%s", "(group,instance) already exists");
      break;
#else
    case PvmNoEntry:
      (void) sprintf(**res, "%s", "no such (group,instance)");
      break;
    case PvmDupEntry:
      (void) sprintf(**res, "%s", "(group,instance) already exists");
      break;
#endif
    default:
      (void) sprintf(**res, "%s", "Unknow error");
      break;
    }
} /* scipvmerror */



#ifdef __STDC__
void 
C2F(scipvmstart)(int *res, char *hostfile, int *l)
#else
void 
C2F(scipvmstart)(res, hostfile, l)
  int *res;
  char *hostfile;
  int *l;
#endif 
{
  struct stat buf;
  char *rd = 0;
  char *path = NULL;
  int argc = 0;
  char *argv[2];

  argv[0] = "";
  argv[1] = (char*)0;
  if (!strcmp(hostfile, "null")) {/* on ne spécifie pas de hostfile */
				 /* on essaye de prendre
				    $HOME/.pvmd.conf, puis
				    $SCI/.pvmd.conf sinon on laisse
				    faire pvmd... */ 
    if (!argc && (rd = getenv("PVM_ROOT")) && (rd = getenv("HOME"))){
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
	(void) fprintf(stderr, "The configuration file\n %s\nis used.\n", path);
      } else {
	(void) fprintf(stderr, "Warning: PVM_ROOT is set, but there exists no\n %s\n", path);
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
	
	(void) fprintf(stderr, "The standard configuration file\n %s\nis used.\n", path);
	argc = 1;
	argv[0] = path;
      } else {
	free(path);
	(void) fprintf(stderr, "Warning: The standard configuration file $SCI/.pvmd.conf was not found.\n We supposed that PVM and scilab are in standard place on your net. (Cf. man pvmd3)\n", path);
      }
    } /* SCI */
  } else {
    if (stat(hostfile, &buf) == -1){
      (void) fprintf(stderr, "%s: No such file or directory\n", hostfile);
    } else {
      argv[0] = hostfile;
      argc = 1;
    }
  }
  *res = pvm_start_pvmd(argc, argv, 1);
} /* scipvmstart */


#ifdef __STDC__
void 
C2F(scipvmhalt)(int *res)
#else
void 
C2F(scipvmhalt)(res)
  int *res;
#endif 
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
} /* scipvmhalt */


#ifdef __STDC__
void 
C2F(scipvmsettimer)(int *res)
#else
void 
C2F(scipvmsettimer)(res)
  int *res;
#endif 
{
  *res = gettimeofday(&t1, NULL);
} /* scipvmsettimer */


#ifdef __STDC__
void 
C2F(scipvmgettimer)(double *res)
#else
void 
C2F(scipvmgettimer)(res)
  double *res;
#endif 
{
  struct timeval t2;

  *res = gettimeofday(&t2, NULL);
  if (*res != -1){
    *res = (double)(t2.tv_sec-t1.tv_sec)*1000000.+(double)(t2.tv_usec-t1.tv_usec);
    t1 = t2;
  }
} /* scipvmsettimer */


#ifdef __STDC__
void 
C2F(scipvmmytid)(int *res)
#else
void 
C2F(scipvmmytid)(res)
  int *res;
#endif 
{
  *res = pvm_mytid();
} /* scipvmmytid */


#ifdef __STDC__
void 
C2F(scipvmexit)(int *res)
#else
void 
C2F(scipvmexit)(res)
  int *res;
#endif 
{
  *res = pvm_exit();
} /* scipvmexit */


#ifdef __STDC__
void 
C2F(scipvmkill)(int *tids, int *n, int *res)
#else
void 
C2F(scipvmkill)(tids, n, res)
  int *tids;
  int *n; 
  int *res;
#endif 
{
  int i;
  for (i = 0; i < *n; ++i) {
    /*    (void) printf("%d:%d:%d\n", *n, i, tids[i]);*/
    res[i] = pvm_kill(tids[i]);
  }
} /* scipvmkill */


#ifdef __STDC__
void 
C2F(scipvmspawn)(char *task,  int *l1, 
		   char *win,   int *l2,
		   char *where, int *l3, 
		   int *ntask,  int *tids, int *res)
#else
void 
C2F(scipvmspawn)(task, l1, win, l2, where, l3, ntask, tids, res)
  char *task;
  int *l1;
  char *win;
  int *l2;
  char *where;
  int *l3;
  int *ntask;
  int *tids;
  int *res;
#endif 
/* [a, b] = pvm_spawn("my_task", 2) or 
   [a, b] = pvm_spawn("my_task", 1, "my_station") */
{
  int flag = PvmTaskDefault;
  char *path, cmd[256];
  char *arg[4];
  
  arg[0] = "";
  arg[1] = "";
  arg[2] = "";
  arg[3] = (char*)0;
  
  /* 
   *res = fprintf(stderr, "%s[%d]:%d:%s[%d]:%d:\n", 
   task, *l1, flag, where, *l3, *ntask); 
  */

  /*
    sciprint( "%s[%d]:%d:%s[%d]:%d:\n",task, *l1, flag, where, *l3, *ntask);
  */
  cmd[0] = 0;

  if (!strcmp(where, "null"))
    where = NULL;
  else
    flag = PvmTaskHost;

#ifdef __MSC__  
  if (path = getenv("WSCI")) {
    strcpy(cmd, path);
    strcat(cmd, "\\bin\\scilex.exe");
  }
  else {
    strcpy(cmd, "scilex");
  }
#else 
  strcpy(cmd, "scilex");
#endif 
#if (defined __MSC__) || (defined __ABSC__)
  if ( _stricmp(task,"null") != 0) {
#else 
  if (strcasecmp(task, "null")) {
#endif
    arg[0] = "-f";
    arg[1] = task;
  }
  if (!strcmp(win, "nw"))
    arg[2] = "-nw";
  
  /*(void) fprintf(stderr, "%s:%s:%s:%s\n", cmd, arg[0],arg[1],arg[2]);*/

  /* sciprint( "%s:%s:%s:%s\n", cmd, arg[0], arg[1],arg[2]); */
  /* *res = pvm_spawn(cmd, arg, flag, where, *ntask, tids); */
  *res = pvm_spawn(cmd, arg, flag, where, *ntask, tids);
  
} /* scipvmspawn */


#ifdef __STDC__
void 
C2F(scipvmspawnindependent)(char *task,  int *l1, 
			       int *ntask,
			       char *where, int *l3, 
			       int *tids, int *res)
#else
void 
C2F(scipvmspawnindependent)(task, l1, ntask, where, l3, tids, res)
  char *task;
  int *l1;
  int *ntask;
  char *where;
  int *l3;
  int *tids;
  int *res;
#endif 
{
/*[a,b] = pvm_spawn_independent("/usr/openwin/bin/xclock", 1,"radagast") */
/*[a,b] = pvm_spawn_independent("/home/ubeda/SCILAB/scilab-2.3/bin/scilab",1)*/
  int flag = PvmTaskDefault;

  /*  *res = fprintf(stderr, "%s[%d]:%d:%s[%d]:%d:\n", 
      task, *l1, flag, where, *l3, *ntask);*/

  if (!strcmp(where,"null"))
    where = NULL;
  else
    flag = PvmTaskHost;

  *res = pvm_spawn(task, NULL, flag, where, *ntask, tids);
} /* scipvmspawnindependent */


#ifdef __STDC__
void 
C2F(scipvmaddhosts)(char ***hosts, int *n, int *infos)
#else
void 
C2F(scipvmaddhosts)(hosts, n, infos)
  char ***hosts;
  int *n;
  int *infos;
#endif 
{
  pvm_addhosts(*hosts, *n, infos);
} /* scipvmaddhosts */


#ifdef __STDC__
void
C2F(scipvmdelhosts)(char ***hosts, int *n, int *infos)
#else
void
C2F(scipvmdelhosts)(hosts, n, infos)
  char ***hosts;
  int *n;
  int *infos;
#endif 
{
  pvm_delhosts(*hosts, *n, infos);
} /* scipvmdelhosts */
