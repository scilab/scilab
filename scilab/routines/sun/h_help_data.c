/* Copyright ENPC */
/*
 * Utility functions to build data for 
 * Scilab help browser 
 * J.P Chancelier 
 *    used by wsci/wmhelp.c ( Windows )
 *    and     xsci/h_help.c ( X Window ) 
 */

#include <stdio.h>
#include <string.h>

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
extern  char  *getenv();
#endif

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

#include "../graphics/Math.h"
#include "h_help.h"

extern void C2F(inffic) __PARAMS((integer * iopt,char *name,integer *nc));
extern int C2F(creadchains) __PARAMS((char *,int *,int *,int *,char *,long int,long int));
extern int C2F(xscion) __PARAMS((int *n));

int  InitHelpDatas_With_Chapter __PARAMS((void));
int  InitHelpDatas_FromScilabVar __PARAMS((void));

/*
 * Help pathname 
 */

#define MAX_PATH_STR 1024
static char Help[MAX_PATH_STR];
static char Buf[MAX_PATH_STR];

/*
 * Functions defined here 
 */
static int NewString();
static void CleanHelpTopics();
static void CleanAproposTopics();

/**************************************
 * Info List for one chapter 
 **************************************/

char   **helpTopicInfo = 0;      /** info for the current chapter **/
int      nTopicInfo = 0;         /** number of items in the current chapter**/
static char    *helpPath = 0;
int      CurrentTopicInfo = -1;   /** chapter number of current help**/

/**************************************
 * help chapters 
 **************************************/

char* helpInfo[MAX_HELP_CHAPTERS];
char* helpPaths[MAX_HELP_CHAPTERS];
int   nInfo = 0;

/**************************************
 * Structure to deal with Apropos 
 * AP.nTopic : number of topics in the Apropos
 *             HelpTopic[i] 
 *             is in chapter HelpPaths[AP.Where[i]]
 **************************************/

Apropos AP ;


/******************************************************************
 * Callback procedure 
 * activate the help ( xless application on the selected topic 
 *****************************************************************/

void HelpActivate(ntopic) 
     int ntopic;
{
  char           *TopicInfo;
  char            Topic[30];
  int             ln, k;
  if (AP.nTopic == 0) 
    {
      /* get keyword as first word of  helpTopicInfo[ntopic] */
      TopicInfo = helpTopicInfo[Min(nTopicInfo-1,Max(ntopic,0))];
    } 
  else 
    {
      int ntopic1= Min(AP.nTopic-1,Max(ntopic,0));
      TopicInfo = AP.HelpTopic[ntopic1];
      helpPath  = helpPaths[ AP.Where[ntopic1]];
    }
  Topic[0] = '\0';
  /* Find Topic after @ */
  /* New version */
  sscanf(TopicInfo,"%*[^@\n]@%s",Topic);
  if (Topic[0] == '\0') {
    /* Set Topic to TopicInfo up to the first blank char */
    /* Old version */
    k = 0;
    ln = strlen(TopicInfo);
    while (k < ln && TopicInfo[k] != ' ') 
    {
      Topic[k] = TopicInfo[k];
      k++;
    }
    Topic[k] = '\0';
  }
  SciCallHelp(helpPath,Topic);
}

void SciCallHelp(helpPath,Topic)
     char *Topic;
     char *helpPath;
{
  /** XXXXX cygwin bash scripts can't execute gcwin32 executable in batch
    up to now : we use the standard windows system function 
    that's why we need the getenv 
    ***/
#ifdef WIN32
  static char format1[]="%s/bin/xless  %s/%s.cat ";
  static char format2[]="xless  %s/%s.cat ";
  char *local = (char *) 0;
  local = getenv("SCI");
  if ( local != (char *) 0)
    {
      char *buf = (char *) MALLOC((strlen(local)+strlen(helpPath)+strlen(Topic)+strlen(format1)+1) * (sizeof(char)));
      if (buf == NULL){ sciprint("Running out of memory, I cannot activate help\n");return;}
      sprintf(buf, format1 ,local, helpPath, Topic);
      /** sciprint("TestMessage : je lance un winsystem sur %s\r\n",buf); **/
      if (winsystem(buf,1))
	sciprint("help error: winsystem failed\r\n");
      FREE(buf);
    }
  else
    {
      char *buf = (char *) MALLOC((strlen(helpPath)+strlen(Topic)+strlen(format2)+1) * (sizeof(char)));
      if (buf == NULL){ sciprint("Running out of memory, I cannot activate help\n");return;}
      /** maybe xless is in the path ? **/
      sprintf(buf, format2, helpPath, Topic);
      /** sciprint("TestMessage : je lance un winsystem sur %s\r\n",buf); **/
      if (winsystem(buf,1))
	sciprint("help error: winsystem failed\r\n");
      FREE(buf);
    }
#else
  int i;
  static char format1[]= "$SCI/bin/xless %s/%s.cat 2> /dev/null &";
  static char format2[]= "cat %s/%s.cat | more ";
  C2F(xscion)(&i);
  if ( i != 0 )
    {
      char *buf = (char *) MALLOC((strlen(helpPath)+strlen(Topic)+strlen(format1)+1) * (sizeof(char)));
      if (buf == NULL){ sciprint("Running out of memory, I cannot activate help\n");return;}
      sprintf(buf,format1,helpPath, Topic);
      system(buf);
      FREE(buf);
    }
  else 
    {
      char *buf = (char *) MALLOC((strlen(helpPath)+strlen(Topic)+strlen(format1)+1) * (sizeof(char)));
      if (buf == NULL){ sciprint("Running out of memory, I cannot activate help\n");return;}
      sprintf(buf,format2, helpPath, Topic);
      system(buf);
      FREE(buf);
    }
#endif
}

/**********************************************
 * seaches the help database for a specific help 
 * return 1 if memory failure occurs 
 * this is for the keyboard  scilab command help xxxx
 **********************************************/

int Sci_Help(name) 
     char *name;
{
  int i,k,cti,aps,ok=0;
  char c_name[MAXTOPIC];
  /** keep tracks of the current status **/
  cti=CurrentTopicInfo ;
  aps=AP.nTopic;
  if ( aps != 0) strcpy(c_name,AP.name);
  if ( Help_Init() == 1) 
    {
      sciprint("Sorry but I cannot use man\r\n");
      return(1);
    }
  for ( i= 0 ; i < nInfo ; i++ ) 
    {
      if (setHelpTopicInfo(i+1) == 1) return(1);
      for (k = 0; k < nTopicInfo; k++)
	{
	  if ( strncmp(name,helpTopicInfo[k],strlen(name))== 0 && 
	       helpTopicInfo[k][strlen(name)]== ' ' )
	    {
	      HelpActivate(k);
	      ok=1;break;
	    }
	}
    }
  /** back to previous state **/
  if ( aps != 0) 
    SetAproposTopics(c_name);
  else if ( cti >=1 && cti <= nInfo) 
    setHelpTopicInfo(cti);
  if ( ok == 0) 
    sciprint("No man for %s\r\n",name);
  return(0);
}



/**********************************************
 * seaches the help database for a specific topic
 * return 1 if memory failure occurs 
 * this is for the keyboard  scilab command help xxxx
 **********************************************/

#ifdef WIN32 
#include "../menusX/wmen_scilab.h" 
#else
#include "../menusX/men_scilab.h"
#endif

int Sci_Apropos(name) 
     char *name;
{
  static char *butn[]= { "Ok","Cancel",NULL};
  int Rep;
  ChooseMenu Ch;
  int i,cti,aps;
  char c_name[MAXTOPIC];
  /** keep tracks of the current status **/
  cti=CurrentTopicInfo ;
  aps=AP.nTopic;
  if ( aps != 0) strcpy(c_name,AP.name);
  /** init Help database **/
  if ( Help_Init() == 1) 
    {
      sciprint("can't use man\r\n");
      return(1);
    }
  /** fills apropos database **/
  if ( SetAproposTopics(name) == 1) return(1);
  if ( AP.nTopic == 0 ) 
    {
      sciprint("Nothing found for topic %s\r\n",name);
      return(0);
    }
  else 
    {
      C2F(xscion)(&i);
      if ( i != 0 ) 
	{
	  Ch.nstrings =   AP.nTopic ;
	  Ch.nb = 2; 
	  Ch.choice = 0;
	  Ch.description = name;
	  Ch.buttonname = butn;
	  Ch.strings = AP.HelpTopic ;
	  Rep = ExposeChooseWindow(&Ch);
	  if ( Rep == TRUE )
	    HelpActivate(Ch.choice);
	}
      else 
	{
	  for ( i = 0 ; i < AP.nTopic ; i++) 
	    sciprint("%s\r\n", AP.HelpTopic[i]);
	}
    }
  /** back to previous state **/
  if ( aps != 0) 
    SetAproposTopics(c_name);
  else if ( cti >=1 && cti <= nInfo) 
    setHelpTopicInfo(cti);
  return(0);
}

/************************************
 * Init function : 
 ************************************/

int Help_Init()
{
  static int first = 0;
  static int Erstatus = 0 ;
  if ( first == 0 ) 
    {
      if (( Erstatus=initHelpDatas()) == 1)  return(1);
      if (( Erstatus=setHelpTopicInfo(1)) == 1) return(1);
      AP.name[0] ='\0' ; /** Apropos topic initial name **/
      first++;
    }
  else
    {
      if ( Erstatus == 1 ) return(1);
      if ((Erstatus = setHelpTopicInfo(1)) == 1) return(1);
    }
  return(0);
}


/************************************
 * Init help Chapters List 
 ************************************/

int initHelpDatas()
{
  /** Help chapters was previously searched in Chapter 
      we use now Scilab variable %helps 
      Note that this variable is just read one time.
  **/
  
      
  /** return initHelpDatas_With_Chapter(); **/
  return  InitHelpDatas_FromScilabVar();
}

int InitHelpDatas_With_Chapter()
{
  static int first=0;
  FILE           *fg;
  char            line[120];
  int             linecount;
  integer         iopt=1, nc;
  /** Init to zero **/
  if ( first == 0) 
    {
      int i ;
      for ( i = 0 ; i < MAX_HELP_CHAPTERS ; i++) 
	{
	  helpInfo[i] = (char *) 0;
	  helpPaths[i]= (char *) 0;
	}
      first = 1;
    }
  /* Get  Help Chapters file path */
  C2F(inffic) (&iopt, line, &nc);
  if ( HelpGetPath(line,Help,Buf) != 0 ) return(1);
  fg = fopen(Help, "r");
  if (fg == 0) 
    {
      sciprint("Unable to open file \"%s\"\r\n", Help);
      nInfo = 0;
      return (1);
    }
  /* Read path and title of each help Chapter */
  linecount = nInfo =  0;
  while (fgets(line, 120, fg) != 0) 
    {
      int rep;
      rep = HelpGetPath(line,Help,Buf) ;
      linecount++;
      if ( rep == -1 ) continue;
      if ( rep == 1  ) 
	{
	  sciprint("Ignoring line %d \r\n",linecount);
	  continue;
	}
      if ( NewString(&helpPaths[nInfo],Help) == 1 
	  ||  NewString(&helpInfo[nInfo],Buf) == 1 )
	{
	  int k;
	  /** Lacking memory for help **/
	  fclose(fg);
	  for (k = 0; k < nInfo ; k++) 
	    {
	      FREE(helpInfo[k]);
	      FREE(helpPaths[k]);
	    }
	  nInfo = 0 ;
	  return(1);
	}
      nInfo++;
      if (nInfo == MAX_HELP_CHAPTERS)
	{
	  sciprint("Max help chapters (%d) reached \r\n",
		   MAX_HELP_CHAPTERS );
	  break;
	}
    }
  fclose(fg);
  helpInfo[nInfo] = NULL;
  helpPaths[nInfo] = NULL;
  /** Init Apropos **/
  AP.nTopic =0;
  /** Init Topics **/
  nTopicInfo = 0;
  return (0);
}

/*********************************************************
 * Same as initHelpDatas, but the pathnames and titles 
 * are  found in a Scilab %help variable 
 *********************************************************/

int InitHelpDatas_FromScilabVar()
{
  static int first=0;
  int  i,ir,ic,itslen= MAX_PATH_STR,Sm;
  /** Init to zero **/
  if ( first == 0) 
    {
      int i ;
      for ( i = 0 ; i < MAX_HELP_CHAPTERS ; i++) 
	{
	  helpInfo[i] = (char *) 0;
	  helpPaths[i]= (char *) 0;
	}
      first = 1;
    }
  ir=ic=-1;/** first call to get size informations **/
  if ( ! C2F(creadchains)("%helps",&ir,&ic,&itslen,Help,strlen("%helps"),MAX_PATH_STR -1))
    {
      sciprint("Failed to find %%helps Scilab variable\r\n");
      nInfo = 0;
      return 1;
    }
  if ( ic != 2 ) 
    {
      sciprint("Error: %helps must be (mx2) not (%dx%d)\r\n",ir,ic);
      nInfo = 0;
      return 1;
    }
  Sm = ir;
  if ( Sm >= MAX_HELP_CHAPTERS ) 
    {
      sciprint("Too many help chapters %d (max=%d),I'll ignore the last ones \r\n",
	       Sm, MAX_HELP_CHAPTERS-1);
    }
  for ( i = 0 ; i < Min(MAX_HELP_CHAPTERS,Sm); i++)
    {
      int one=1,two =2,row;
      row= i+1;
      itslen= MAX_PATH_STR ;
      C2F(creadchains)("%helps",&row,&one,&itslen,Help,strlen("%helps"),MAX_PATH_STR -1);
      itslen= MAX_PATH_STR ;
      C2F(creadchains)("%helps",&row,&two,&itslen,Buf,strlen("%helps"),MAX_PATH_STR -1);
      /** XXXX faire le expand **/
      if ( NewString(&helpPaths[i],Help) == 1 
	  ||  NewString(&helpInfo[i],Buf) == 1 )
	{
	  int k;
	  /** Lacking memory for help **/
	  for (k = 0; k < i ; k++) 
	    {
	      FREE(helpInfo[k]);
	      FREE(helpPaths[k]);
	    }
	  nInfo =0;
	  return 1;
	  break;
	}
    }
  nInfo = Sm ;
  helpInfo[i] = NULL;
  helpPaths[i] = NULL;
  /** Init Apropos **/
  AP.nTopic =0;
  /** Init Topics **/
  nTopicInfo = 0;
  return 0;
}


    
/************************************
 * Parses a line as 
 * $SCI/man/graphics	Graphic Library
 * expand and stores the pathname in Path 
 * and the Title in Tit 
 * ( leading ' ' are ignored )
 ************************************/

int HelpGetPath(line,Path,Tit)
     char *line,*Tit,*Path;
{
  char *local = (char *) 0, lk;
  int ln,k;
  ln = strlen(line);
  if (line[ln - 1] == '\n') line[ln - 1] = '\0';
  while ( *line == ' ' ) line++;
  if ( *line == '\0' ) return(-1);
  if ( *line == '$') 
    { 
      int k1=0;
      /* path begins with an environnement variable */
      while (line[k1] != '/' && line[k1] != '\0' && line[k1] != ' ') k1++;
      lk = line[k1];
      line[k1] = '\0';
      if ((local = getenv(line+1)) == NULL) 
	{
	  sciprint("You must define the environment variable %s\r\n",
		   line+1);
	  return(1);
	} 
      else 
	{
	  line = line + k1;
	  *line = lk;
	}
    } 
  /* get rest of path */
  k=0;
  while ( line[k] != ' ' && line[k] != '\t' && line[k] != '\0') k++;
  /* store path */
  lk = line[k]; line[k] = '\0';
  if ( local != (char *) 0) {
      strcpy(Path,local);
      strcat(Path,line);
  }
  else {
      strcpy(Path,line);
  }
  /* Store Title */
  line[k]=lk;
  if ( line[k] != '\0' ) 
    {
      while ( line[k] == ' ' || line[k] == '\t' ) k++;
      strcpy(Tit,&line[k]);
    }
  else
    {
      Tit[0] ='\0';
    }
  return(0);
}

/************************************
 * Get The List of helps info 
 * for chapter n 
 ************************************/

int setHelpTopicInfo(n)
     int n;
{
  FILE           *fg;
  char            line[120];
  int             linecount, k, ln;
  CleanAproposTopics();
  if ( n == CurrentTopicInfo ) 
    {
      return(0);
    }
  CleanHelpTopics();
  helpPath = helpPaths[n - 1];
  strcpy(Help, helpPath);
  strcat(Help, "/whatis");
  fg = fopen(Help, "r");
  if (fg == 0)
    {
      sciprint("Unable to open file \"%s\"\r\n", Help);
      return(1);
    }
  /* read whatis file just to count lines */
  linecount = 0;
  while (fgets(line, 120, fg) != 0)
    linecount++;
  rewind(fg);

  /* allocate a new helpTopicInfo */
  helpTopicInfo = (char **) MALLOC((linecount + 1) * sizeof(char *));
  if (helpTopicInfo == NULL) {
    sciprint("Not enough memory to allocate help tables\r\n");
    return(1);
  }
  nTopicInfo = 0;
  while (fgets(line, 120, fg) != 0) 
    {
      ln = strlen(line);
      if (line[ln - 1] == '\n') line[ln - 1] = '\0';
      if ( NewString(&( helpTopicInfo[nTopicInfo]),line) == 1)
	{
	  fclose(fg);
	  for (k = 0; k < nTopicInfo; k++)
	    FREE(helpTopicInfo[k]);
	  FREE(helpTopicInfo);
	  nTopicInfo = 0;
	  return(1);
	}
      nTopicInfo++;
    }
  helpTopicInfo[nTopicInfo]= (char *) 0;
  CurrentTopicInfo = n ;
  fclose(fg);
  return(0);
}


/******************************************
 * Clean The HelpInfo Datas  
 ******************************************/

static void CleanHelpTopics()
{
  int k;
  for (k = 0; k < nTopicInfo; k++)
    FREE(helpTopicInfo[k]);
  FREE(helpTopicInfo);
  nTopicInfo = 0;
  CurrentTopicInfo = -1;
}

/******************************************
 * Set the Apropos structure AP
 * with whatis line refering to str 
 * In case of memory failure the routine return 1
 ******************************************/

int SetAproposTopics(str)
     char *str;
{
  FILE           *fg;
  char            line[120];
  int             k, ln, n;
#if (defined __MSC__) || (defined __ABSC__)
  if ( _stricmp(str,AP.name)== 0) 
#else 
  if ( strcasecmp(str,AP.name)== 0) 
#endif
    {
      /** current apropos database is correct **/
      return(0);
    }
  CleanAproposTopics();
  AP.nTopic = 0;
  for (n = 0; n < nInfo; n++) 
    {	
      strcpy(Help, helpPaths[n]);
      strcat(Help, "/whatis");
      fg = fopen(Help, "r");
      if (fg == 0) 
	{
	  sciprint("Unable to open file \"%s\"\r\n", Help);
	  continue;
	}
      /* search for str in  whatis file */
      while (fgets(line, 120, fg) != 0) 
	{
	  if (strstr(line, str) != 0) 
	    {
	      ln = strlen(line);
	      if (line[ln - 1] == '\n')
		line[ln - 1] = '\0';
	      if ( NewString(&(AP.HelpTopic[AP.nTopic]),line)==1)
		{
		  fclose(fg);
		  for (k = 0; k < AP.nTopic ; k++)
		    FREE(AP.HelpTopic[k]);
		  AP.nTopic =0;
		  return(1);
		}
	      AP.Where[AP.nTopic] = n;
	      AP.nTopic++;
	      if ( AP.nTopic >= APROPOSMAX-1)
		{
		  sciprint("Too many answers for topic %s \r\n",str);
  		  sciprint("I will ignore the last ones \r\n");
		break;
	    }
	}
	}
      fclose(fg);
    }
  AP.HelpTopic[AP.nTopic] = NULL;
  strncpy(AP.name,str,MAXTOPIC);
  return(0);
}

/******************************************
 * Clean The Apropos Datas  
 ******************************************/

static void CleanAproposTopics()
{
  int n;
  for (n = 0; n < AP.nTopic ; n++) 
    {
      FREE(AP.HelpTopic[n]);
    }
  AP.nTopic = 0;
  AP.name[0] = '\0';
}

/************************************
 * Utility function 
 ************************************/

static int NewString(hstr,line)
     char **hstr, *line;
{
  *hstr = (char *) MALLOC((strlen(line) + 1) * (sizeof(char)));
  if ( (*hstr) == NULL)
    {
      sciprint("Not enough memory to allocate help tables\r\n");
      return(1);
    }
  strcpy(*hstr, line);
  return(0);
}





