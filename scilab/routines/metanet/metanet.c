/* Copyright INRIA */
#include "../machine.h"

#include <errno.h>
#if defined(SYSV) || defined(WIN32)
#include <string.h>
#else
#include <strings.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../libcomm/libCalCom.h"
#include "../libcomm/libCom.h"
#include "netcomm.h"

#if (defined __MSC__) || (defined __ABSC__)
/** gethostname exists metanet not implemented in windows **/
/**void gethostname(char *str,int len) { 
  strncpy(str,"BUG",len);
} **/
#include <winsock2.h>
/** only used for x=dir[1024] **/
#ifndef __ABSC__
#define  getwd(x) _getcwd(x,1024); 
#else
#define getwd(x) getcwd_(x,1024)
#endif
#endif

#ifdef __MINGW32__ 
#include <dir.h>
#include <windows.h>
int PASCAL gethostname( char * name,  int namelen  );

#define  getwd(x) _getcwd(x,1024); 
#endif

extern void cerro();
extern void Cout();

extern int isGeci;

#define MAXNETWINDOW 512

char *Netwindows[MAXNETWINDOW];
int theNetwindow = 0;
int nNetwindows = 0;
int metanetSync = 0;

#define MAXNAM 80
#define MAXHOSTLEN 128

static char str[MAXNAM];

void C2F(inimet)(scr,path,lpath,dwidth,dheight)
int *scr;
char *path;
int *lpath;
int *dwidth, *dheight;
{
  char windowname[MAXNAM];
  char command[2 * MAXNAM];
  char warg[MAXNAM], widtharg[MAXNAM], heightarg[MAXNAM];
  char* env;
  char server[MAXHOSTLEN];
  char dir[1024];

  if (isGeci == 0) {
    cerro("Unable to launch Metanet graphics window: no communications");
    return;
  }
  path[*lpath] = '\0';

  strcpy(dir,path);
  if (!strcmp(path," ")) getwd(dir);

  gethostname(server,MAXHOSTLEN);
  
  scanner_messages();
  nNetwindows++;

   if (nNetwindows > MAXNETWINDOW) {
    cerro("Too many windows");
    return;
  }

  sprintf(windowname,"Metanet%d", nNetwindows);

  if ((Netwindows[nNetwindows - 1] = 
       (char *)malloc((strlen(windowname)+1) * sizeof(char))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  strcpy(Netwindows[nNetwindows - 1],windowname);

  env = getenv("XMETANET");
  if (env == NULL) {
    env = getenv("SCI");
    if (env == NULL) {
      cerro("The environment variable SCI is not defined");
      return;
    }
    else sprintf(command,"%s/bin/xmetanet",env);
  }
  else sprintf(command,"%s",env);

  sprintf(warg,"%d",nNetwindows);
  sprintf(widtharg,"%d",*dwidth);
  sprintf(heightarg,"%d",*dheight);

  envoyer_message_parametres_var(ID_GeCI,
				 MSG_LANCER_APPLI,
				 windowname,
				 server,
				 command,
				 "-w", warg,
				 dir,
				 widtharg, heightarg,
				 "__ID_PIPES__",
				 NULL);

  if (theNetwindow != 0) {
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_DETRUIRE_LIAISON, 
				   identificateur_appli(),
				   Netwindows[theNetwindow - 1],NULL);
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_DETRUIRE_LIAISON, 
				   Netwindows[theNetwindow - 1],
				   identificateur_appli(),NULL);
  }

  theNetwindow = nNetwindows;

  envoyer_message_parametres_var(ID_GeCI,
				 MSG_CREER_LIAISON, 
				 identificateur_appli(),
				 Netwindows[theNetwindow - 1],NULL);
  envoyer_message_parametres_var(ID_GeCI,
				 MSG_CREER_LIAISON, 
				 Netwindows[theNetwindow - 1],
				 identificateur_appli(),NULL);

  *scr = theNetwindow;
}

/* checkNetconnect and checkTheNetwindow must be called before using XMetanet
   typically you put at the beginning of each function speaking to XMetanet:
   if (!checkNetconnect() || !checkTheNetwindow()) return;
*/

int checkNetconnect()
{
  /* checking for closed Metanet windows */
  scanner_messages();
  if (nNetwindows == 0) {
    cerro("You must first execute Metanet");
    return 0;
  }
  return 1;
}

int checkTheNetwindow()
{
  if (theNetwindow == 0) {
    cerro("The current window is closed");
    return 0;
  }
  return 1;
}

void C2F(netwindow)(s)
int *s;
{
  char str[MAXNAM];

  if (checkNetconnect() == 0) return;

  if (*s > nNetwindows || *s < 1) {
    sprintf(str,"Bad window number: %d",*s);
    cerro(str);
    return;
  }

  if (strcmp(Netwindows[*s - 1],"CLOSED") == 0) {
    sprintf(str,"Window number %d is closed",*s);
    cerro(str);
    return;
  }
  
  if (theNetwindow != 0) {
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_DETRUIRE_LIAISON, 
				   identificateur_appli(),
				   Netwindows[theNetwindow - 1],NULL);
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_DETRUIRE_LIAISON, 
				   Netwindows[theNetwindow - 1],
				   identificateur_appli(),NULL);
  }

  theNetwindow = *s;
  envoyer_message_parametres_var(ID_GeCI,
				 MSG_CREER_LIAISON, 
				 identificateur_appli(),
				 Netwindows[theNetwindow - 1],NULL);
  envoyer_message_parametres_var(ID_GeCI,
				 MSG_CREER_LIAISON, 
				 Netwindows[theNetwindow - 1],
				 identificateur_appli(),NULL);
}

void C2F(netwindows)(vs,nvs,cv)
int **vs;
int *nvs;
int *cv;
{
  int i,j;
  int s[MAXNETWINDOW];

  scanner_messages();

  *nvs = 0;
  *cv = 0;
  if (nNetwindows == 0) return;

  j = 0;
  for (i = 1; i <= nNetwindows; i++ ) {
    if (strcmp(Netwindows[i - 1],"CLOSED") != 0)
      s[j++] = i;
  }

  *nvs = j;
  if (j == 0) return;

  if ((*vs = (int *)malloc(*nvs * sizeof(int))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  for (i = 0; i < *nvs; i++){
    (*vs)[i] = s[i];
  }
  *cv = theNetwindow;
}

void CloseNetwindow(s)
int s;
{
  if ((Netwindows[s - 1] = 
       (char *)malloc((strlen("CLOSED")+1) * sizeof(char))) == NULL) {
    cerro("Running out of memory");
    return;
  }
  strcpy(Netwindows[s - 1],"CLOSED");

  if (theNetwindow == s) {
    sprintf(str,"Warning: current Metanet window %d has been closed",s);
    Cout(str);
    theNetwindow = 0;
  }
}

void C2F(metasync)(s,res)
int *s;
int *res;
{
  if (*s == -1) {
    *res = metanetSync;
  } else {
    metanetSync = *s;
    *res = *s;
  }
}

void C2F(netclose)(s)
int *s;
{
  char str[MAXNAM];

  if (checkNetconnect() == 0) return;

  if (*s > nNetwindows || *s < 1) {
    sprintf(str,"Bad window number: %d",*s);
    cerro(str); 
    return;
  }

  if (strcmp(Netwindows[*s - 1],"CLOSED") == 0) {
    sprintf(str,"Window number %d is closed",*s);
    cerro(str); 
    return;
  }
  
  if (theNetwindow != 0 & theNetwindow != *s) {
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_DETRUIRE_LIAISON, 
				   identificateur_appli(),
				   Netwindows[theNetwindow - 1],NULL);
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_DETRUIRE_LIAISON, 
				   Netwindows[theNetwindow - 1],
				   identificateur_appli(),NULL);

    envoyer_message_parametres_var(ID_GeCI,
				   MSG_CREER_LIAISON, 
				   identificateur_appli(),
				   Netwindows[*s - 1],NULL);
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_CREER_LIAISON, 
				   Netwindows[*s - 1],
				   identificateur_appli(),NULL);
  }

  SendMsg(CLOSE,"");

  if (theNetwindow != 0 & theNetwindow != *s) {
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_CREER_LIAISON, 
				   identificateur_appli(),
				   Netwindows[theNetwindow - 1],NULL);
    envoyer_message_parametres_var(ID_GeCI,
				   MSG_CREER_LIAISON, 
				   Netwindows[theNetwindow - 1],
				   identificateur_appli(),NULL);  

  }
}
