/* Copyright INRIA */
/***************************************************************************/
#include <stdio.h>
#include <string.h>

/* Communications headers */
#include "/usr/local/lib/scilab-2.3/routines/libcomm/libCalCom.h"
#include "/usr/local/lib/scilab-2.3/routines/libcomm/libCom.h"

static void QuitAppli();  
static void EndAppli();
static void ParseMessage();
static void MsgError();

/* Known messages */
static actions_messages tb_messages[]={
  {ID_GeCI,MSG_QUITTER_APPLI,NBP_QUITTER_APPLI,QuitAppli},
  {ID_GeCI,MSG_FIN_APPLI,NBP_FIN_APPLI,EndAppli},
  {NULL,MSG_DISTRIB_LISTE_ELMNT,NBP_DISTRIB_LISTE_ELMNT,ParseMessage},
  {NULL,NULL,0,MsgError}};

static void QuitAppli(message)
     Message message;
{  
  printf("Quit application\n");
  exit(0);
}

static void EndAppli(message) 
     Message message;
{
  printf("End application\n");
}

static void MsgError(message)
     Message message;
{
  printf("Bad received message\n");
}

static char *TheAppli;
static char *TheType;
static char *TheMsg;

/* ParseMessage is executed when a message is received */
static void ParseMessage(message)
     Message message;
{
  int lappli, ltype, lmsg;

  lappli = strlen(message.tableau[0]);
  if ((TheAppli = (char *)malloc((unsigned)sizeof(char)*(lappli + 1)))
      == NULL) {
    return;
  }
  strcpy(TheAppli,message.tableau[0]);

  ltype = strlen(message.tableau[3]);
  if ((TheType = (char *)malloc((unsigned)sizeof(char)*(ltype + 1)))
      == NULL) {
    return;
  }
  strcpy(TheType,message.tableau[3]);

  lmsg = strlen(message.tableau[4]);
  if ((TheMsg = (char *)malloc((unsigned)sizeof(char)*(lmsg + 1)))
      == NULL) {
    return;
  }
  strcpy(TheMsg,message.tableau[4]);
}

static int find(s,n,t)
     char *s;
     int n;
     char **t;
{
  int i;
  for (i=0; i<n; i++)
    if (!strcmp(s,t[i])) return(i);
  return(-1);
}

int main(argc, argv)
     unsigned int argc;
     char **argv;
{
  int igeci;
  int p1, p2;
  char myhost[128];
  /* Scilab application to execute */
  char *scilex = "/usr/local/lib/scilab-2.3/bin/scilex";

  igeci = find("-pipes",argc,argv);
  if (igeci == -1) exit(1);

  p1 = atoi(argv[igeci+1]); p2 = atoi(argv[igeci+2]);

  /* Intialization of communications */
  init_messages(tb_messages,p1,p2);

  /* Get the name of my computer */
  gethostname(myhost,128);

  /* Execute Scilab with name "Scilab" on my local host */
  envoyer_message_parametres_var(ID_GeCI,
                                 MSG_LANCER_APPLI,
                                 "Scilab",
                                 myhost,
                                 scilex,
                                 INS_ID_PIPES,
                                 NULL);

  /* Link THIS application with "Scilab" */
  envoyer_message_parametres_var(ID_GeCI,
                                 MSG_CREER_LIAISON, 
                                 identificateur_appli(),
                                 "Scilab",NULL);
  
  /* Link "Scilab" with THIS application  */
  envoyer_message_parametres_var(ID_GeCI,
                                 MSG_CREER_LIAISON, 
                                 "Scilab",
                                 identificateur_appli(),NULL);

  /* Loop waiting for messages */
  while (1) {
    scanner_messages();
    if (TheType != NULL) {
      printf("Message received from %s\n",TheAppli);
      printf("    type: %s\n",TheType);
      printf("    message: %s\n",TheMsg);
      TheAppli = NULL; TheType = NULL; TheMsg = NULL;
    }
  }
}
/***************************************************************************/
