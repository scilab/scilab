/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *    Changed: steer, jpc 2004 
 *--------------------------------------------------------------------------*/
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <sys/utsname.h>
#endif

#include <string.h>
#include "../machine.h"
#include "../graphics/Math.h"  /* malloc */
#include "../stack-c.h"
#include "../version.h"

#ifdef WIN32
#include "../Wsci/WinConsole.h"
#endif

#ifndef NULL
#define NULL 0
#endif

#define TRUE  1
#define FALSE 0
#define MAX_PATH_LONG 32767
#define MAX_PATH_SHORT 260


/*---------------------------------------------------------------------
 *  Command queue functions
 *  This function is used to store Scilab command in a queue 
 *  ( the queue is checked in the Scilab Event Loop )
 *  The queue is filled by the function related to dynamic buttons and menus 
 *  One can also set a handler to deal with the commands 
 *  if he wants to bypass the queue 
 *
 *  PUBLIC : set_scig_command_handler(Scig_command_handler f)
 *           void reset_scig_command_handler() 
 *           int StoreCommand( char *command)
 *           integer C2F(ismenu)()
 *           int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)  
 *---------------------------------------------------------------------*/

#ifdef WIN32
extern int GetOSVersion(void);
extern int HideToolBarWin32(int WinNum); /* see "wsci/wmenu.c" */
extern BOOL IsToThePrompt(void);
extern void SetLanguageMenu(char *Language); /* see "wsci/wmenu.c" */
extern int InterfaceWindowsQueryRegistry _PARAMS((char *fname));
extern int	InterfaceWindowsClipboard _PARAMS((char *fname));
extern int	InterfaceWindowsDDEopen _PARAMS((char *fname));
extern int	InterfaceWindowsDDEclose _PARAMS((char *fname));
extern int	InterfaceWindowsDDEexec _PARAMS((char *fname));
extern int	InterfaceWindowsDDEpoke _PARAMS((char *fname));
extern int	InterfaceWindowsDDEreq _PARAMS((char *fname));
extern int	InterfaceWindowsDDEIsOpen _PARAMS((char *fname));
extern int ToolBarWin32(int WinNum,char *onoff);
extern int GetStateToolBarWin32(int WinNum);
#endif /*WIN32*/

extern void banner(void);

extern void write_scilab  __PARAMS((char *s));
extern int get_is_reading  __PARAMS((void));
extern void sciprint __PARAMS((char *fmt,...));


typedef int (*Scig_command_handler) __PARAMS((char *));
extern Scig_command_handler set_scig_command_handler __PARAMS((Scig_command_handler f));
int  scig_command_handler_none (char *command);
extern int StoreCommand  __PARAMS((char *command));  
extern int StoreCommand1  __PARAMS((char *command, int flag));  

extern int GetCommand  __PARAMS((char *str));  
extern integer C2F (ismenu) __PARAMS((void));  
extern int C2F (getmen) __PARAMS((char *btn_cmd, integer *lb, integer *entry));  
extern void  reset_scig_command_handler __PARAMS((void));

static int SaveHistoryAfterNcommands=0;
static int wait_for_input_end=0; 

extern int NumberOfCommands;


typedef struct commandRec
{
  char              *command;		/* command info one string two integers */
  int               flag; /* 1 if the command execution cannot be interrupted */
  struct commandRec *next;
} CommandRec, *CommandRecPtr;

static CommandRec *commandQueue = NULL;
static Scig_command_handler scig_command_handler = scig_command_handler_none;


/*-------------------------------------------------*
 * changing the default command handler 
 *-------------------------------------------------*/

int  scig_command_handler_none (char *command) {return 0;}

Scig_command_handler set_scig_command_handler (Scig_command_handler f)
{
  Scig_command_handler old = scig_command_handler;
  scig_command_handler = f;
  return old;
}

void reset_scig_command_handler ()
{
  scig_command_handler = scig_command_handler_none;
}

/*---------------------------------------------------------------
 * try to execute a command or add it to the end of command queue 
 *----------------------------------------------------------------*/
int StoreCommand ( char *command)
{
	return (StoreCommand1 (command, 0)); /* jpc 1->0 */
}

/*---------------------------------------------------------------
 * try to execute a command or add it to the end of command queue
 * flag = 0 : the command is not shown in scilab window
 * flag = 1 : the command is shown in scilab window (if at prompt) 
 *----------------------------------------------------------------*/

int StoreCommand1 (char *command,int flag)
{
#ifdef WIN32
 if ( (flag == 1) && ( !IsToThePrompt () ) ) flag=0;
#endif
 switch (flag)
 {
 case 1: /* the command is shown in scilab window (if at prompt) */
		{
			write_scilab (command);
			return (0);
		}
		break;
 case 0: default : /* the command is not shown in Scilab */
		{
			  CommandRec *p, *q, *r;

			  /** first check if we have a special handler set for commands **/
			  if (scig_command_handler (command) == 1)  return 0;

			  p = (CommandRec *) malloc (sizeof (CommandRec));
			  if (p == (CommandRec *) 0)
				{
					sciprint ("send_command : No more memory \r\n");
					return (1);
				}
			  p->flag = 0;
			  p->command = (char *) malloc ((strlen (command) + 1) * sizeof (char));
			  if (p->command == (char *) 0)
				{
				  free(p);
				  sciprint ("send_command : No more memory \r\n");
				  return (1);
				}
			  strcpy (p->command, command);
			  p->next = NULL;
			  if (!commandQueue) commandQueue = p;
			  else
				{
					q = commandQueue;
					while ((r = q->next))	q = r;
					q->next = p;
				}
#ifdef WIN32
			if (IsToThePrompt ()) write_scilab ("\n");
#endif
			return (0);
			
		break;
		}
 }
return (0);
}
void SetCommandflag(int flag)
{
  CommandRec *p, *r;
  if (commandQueue != NULL) {
    p = commandQueue;
    while ((r = p->next))	p = r;
    p->flag=flag;
  }
}

/*-------------------------------------------------
 * Gets info on the first queue element 
 * and remove it from the queue 
 -------------------------------------------------*/

int GetCommand ( char *str)
{
  int flag;
  flag = 0;	
  if (commandQueue != NULL)
    {

      CommandRec *p;

          	
      p = commandQueue;
      strcpy (str, p->command);
      flag=p->flag;

      commandQueue = p->next;
      free (p->command);
      free (p);
      if (C2F(iop).ddt==-1) {
        if (flag==0) { sciprint("   Unqueuing %s - No option\r\n",str); }
        else         { sciprint("   Unqueuing %s - seq\r\n",str); }
      }
    }
  return flag;
}

/*-------------------------------------------------
 * Checks if there's something on the 
 * commandQueue 
 *-------------------------------------------------*/

integer C2F(ismenu)()
{
  if ( (commandQueue == NULL) || (C2F(com).comp[0] != 0)) 
    return(0) ;
  else 
    return(1);
}

/*-------------------------------------------------
 * menu/button info for Scilab 
 *-------------------------------------------------*/

int C2F(getmen)(char * btn_cmd,integer * lb, integer * entry)
{
  int flag;
  if (C2F(ismenu)()==1) 
    {
      flag=GetCommand(btn_cmd);
      *lb=(integer)strlen(btn_cmd);
      *entry=0;  /* This parameter entry seems to be unused. Probably a very old thing... */
    }
  else
    { 
      flag=0;
      *lb =0;
      *entry=0;  /* This parameter entry seems to be unused. Probably a very old thing... */
    }
  return flag;
}


/*-------------------------------------------------
 * ??????????????? 
 *-------------------------------------------------*/


void C2F(waitforinputend)( int *flag)
{
  wait_for_input_end=*flag;
}


int iswaitingforinputend()
{
  int iwait;
  iwait=wait_for_input_end;
  wait_for_input_end=0;
  return iwait;
}
/*-----------------------------------------------------------------------------------*/
/* Open Web Browser Allan CORNET*/
int C2F(winopen) _PARAMS((char *fname))
{
	#ifndef WIN32
	#define FILENAME_MAX 4096 
	#endif
	extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len)); 
	char filename[FILENAME_MAX];
	int m1,n1,l1;
	int out_n;
	long int lout;
	#ifdef WIN32
	HINSTANCE error=NULL;
	#endif
	CheckRhs(1,1);
  
	/*  checking variable file */
	GetRhsVar(1,"c",&m1,&n1,&l1);
	/*** first call to get the size **/
	lout=FILENAME_MAX;
	C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);

	#ifdef WIN32
	error = ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);
	if ( error<= (HINSTANCE)32)
	{
		Scierror(999,"Couldn't Open this document\r\n");
		return 0;
	}
	else LhsVar(0)=0;
	#else
		LhsVar(0)=0;
	#endif

	C2F(putlhsvar)();
return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(hidetoolbar) _PARAMS((char *fname))
{
 static int l1, m1, n1;	

 CheckRhs(1,1);
 CheckLhs(1,1);

 if ( GetType(1) == sci_matrix )
 {
	int num_win=-2;

	GetRhsVar(1,"i",&m1,&n1,&l1);
 	num_win=*istk(l1);
	#ifdef WIN32
		HideToolBarWin32(num_win); /* see "wsci/wmenu.c" */
	#endif /*WIN32*/
	LhsVar(0)=0;
	C2F(putlhsvar)();
 }
 else
 {
	Scierror(999,"Parameter incorrect type.\n");
	return 0;
 }
 return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(setlanguagemenu) _PARAMS((char *fname))
{
	#define LANGUAGE_MAX 1024 
	extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len)); 
	char Language[LANGUAGE_MAX];
	int m1,n1,l1;
	int out_n;
	long int lout;

	CheckRhs(1,1);
  
	/*  checking variable file */
	GetRhsVar(1,"c",&m1,&n1,&l1);
	/*** first call to get the size **/
	lout=LANGUAGE_MAX;
	C2F(cluni0)(cstk(l1), Language, &out_n,m1*n1,lout);

	if ( (strcmp(Language,"eng")!=0) && (strcmp(Language,"fr")!=0) )
	{
		sciprint("Unsupported language\n");
		LhsVar(0)=0;
		C2F(putlhsvar)();
		return 0;
	}

	#ifdef WIN32
		SetLanguageMenu(Language);
	#else

	#endif

	LhsVar(0)=0;
	C2F(putlhsvar)();

	return 0;
}
 /*-----------------------------------------------------------------------------------*/
int savehistoryafterncommands(int N)
{
	int SaveHistoryAfterNcommandsTemp=0;

  	SaveHistoryAfterNcommandsTemp=N;
		
	if (SaveHistoryAfterNcommandsTemp>=0)
	{
		SaveHistoryAfterNcommands=SaveHistoryAfterNcommandsTemp;
		NumberOfCommands=0;
	}

	return 0;	
}
/*-----------------------------------------------------------------------------------*/
int GetSaveHistoryAfterNcommands(void)
{
	return SaveHistoryAfterNcommands;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(winqueryreg) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsQueryRegistry(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(clipboard) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsClipboard(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intddeopen) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsDDEopen(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intddeclose) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsDDEclose(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intddeexec) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsDDEexec(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intddepoke) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsDDEpoke(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intddereq) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsDDEreq(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intddeisopen) _PARAMS((char *fname))
{
#ifdef WIN32
	InterfaceWindowsDDEIsOpen(fname);
#else
	Scierror(999,"Only for Windows\r\n");
	return 0;
#endif
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intsleep) _PARAMS((char *fname))
{
	integer m1,n1,l1,sec=0;

	CheckRhs(-1,1);
	if (Rhs == 1) 
	{ 
		GetRhsVar(1,"d",&m1,&n1,&l1);
		CheckScalar(1,m1,n1);
		sec = (integer) *stk(l1);
	
	#ifdef WIN32
		{
			int ms = (sec); /** time is specified in milliseconds in scilab**/
			if (ms != 0) Sleep(ms); /* Number of milliseconds to sleep. */
		}
	#else
		{
			unsigned useconds;
			useconds=(unsigned) sec;
			if (useconds != 0)  
				#ifdef HAVE_USLEEP
					{ usleep(useconds); }
				#else
					#ifdef HAVE_SLEEP
					{  sleep(useconds/1000); }
					#endif
				#endif
		}
	#endif
     }
	 LhsVar(1)=0;
	 C2F(putlhsvar)();
	 return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2004 */
int C2F(intgetos) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	char OperatinSystem[256];
	char Release[256];
    char *output=NULL;

	#ifndef WIN32
	struct utsname uname_pointer;
	#endif

	Rhs=Max(0,Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

#if WIN32
	
	#define OS_ERROR                       -1
	#define OS_WIN32_WINDOWS_NT_3_51        0
	#define OS_WIN32_WINDOWS_NT_4_0         1
	#define OS_WIN32_WINDOWS_95			    2
	#define OS_WIN32_WINDOWS_98				3
	#define OS_WIN32_WINDOWS_Me				4
	#define OS_WIN32_WINDOWS_2000			5
	#define OS_WIN32_WINDOWS_XP				6
	#define OS_WIN32_WINDOWS_SERVER_2003_FAMILY 7

	sprintf(OperatinSystem,"%s","Windows");

	switch (GetOSVersion())
	{
		case OS_ERROR : default :
			sprintf(Release,"%s","Unknow");
			break;
		case OS_WIN32_WINDOWS_NT_3_51:
			sprintf(Release,"%s","NT 3.51");
			break;
		case OS_WIN32_WINDOWS_NT_4_0:
			sprintf(Release,"%s","NT 4.00");
			break;
		case OS_WIN32_WINDOWS_95:
			sprintf(Release,"%s","95");
			break;
		case OS_WIN32_WINDOWS_98:
			sprintf(Release,"%s","98");
			break;
		case OS_WIN32_WINDOWS_Me:
			sprintf(Release,"%s","ME");
			break;
		case OS_WIN32_WINDOWS_2000:
			sprintf(Release,"%s","2000");
			break;
		case OS_WIN32_WINDOWS_XP:
			sprintf(Release,"%s","XP");
			break;
		case OS_WIN32_WINDOWS_SERVER_2003_FAMILY:
			sprintf(Release,"%s","2003");
			break;
	}
#else
	uname(&uname_pointer);
	sprintf(OperatinSystem,"%s",uname_pointer.sysname);
	sprintf(Release,"%s",uname_pointer.release);
#endif

	
	output=(char*)malloc((strlen(OperatinSystem)+1)*sizeof(char));
	sprintf(output,"%s",OperatinSystem);
	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(output), &m1),&n1,&output);
	if (output) {free(output);output=NULL;}
	LhsVar(1)=1;

	if (Lhs==2)
	{
		char *output2=NULL;
		output2=(char*)malloc((strlen(Release)+1)*sizeof(char));
		sprintf(output2,"%s",Release);
		n1=1;
		CreateVarFromPtr( 2, "c",(m1=(int)strlen(output2), &m1),&n1,&output2);
		if (output2) {free(output2);output2=NULL;}
		LhsVar(2)=2;
	}
	
	C2F(putlhsvar)();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
int C2F(intgetlongpathname) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int bOK=FALSE;

	CheckRhs(0,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		char *LongName=NULL;
		char *ShortName=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		ShortName=cstk(l1);

		LongName=(char*)malloc(MAX_PATH_LONG*sizeof(char));

		#ifdef WIN32
		  if (GetLongPathName(ShortName,LongName,MAX_PATH_LONG))
		  {
			  bOK=TRUE;
		  }
		  else
		  {
			  strcpy(LongName,ShortName);
			  bOK=FALSE;
		  }
		#else
		  strcpy(LongName,ShortName);
		  bOK=FALSE;
		#endif

		n1=1;
		CreateVarFromPtr( 2, "c",(m1=(int)strlen(LongName), &m1),&n1,&LongName);
		LhsVar(1)=2;

		if (LongName) {free(LongName);LongName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)malloc(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(3, "b", &n1, &n1, &bOkOutINT);
			LhsVar(2)=3;
			if (bOkOutINT) {free(bOkOutINT);bOkOutINT=NULL;}
		}

		C2F(putlhsvar)();
	
	}
	else 
	{
		Scierror(999,"Not a path string");
		return 0;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
int C2F(intgetshortpathname) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int bOK=FALSE;

	CheckRhs(0,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		char *LongName=NULL;
		char *ShortName=NULL;
		
		GetRhsVar(1,"c",&m1,&n1,&l1);
		LongName=cstk(l1);

		ShortName=(char*)malloc((strlen(LongName)+1)*sizeof(char));

		#ifdef WIN32
		  if (GetShortPathName(LongName,ShortName,(DWORD)strlen(LongName)+1))
		  {
			  bOK=TRUE;
		  }
		  else
		  {
			  strcpy(ShortName,LongName);
			  bOK=FALSE;
		  }
		#else
		  strcpy(ShortName,LongName);
		  bOK=FALSE;
		#endif

		n1=1;
		CreateVarFromPtr( 2, "c",(m1=(int)strlen(ShortName), &m1),&n1,&ShortName);
		LhsVar(1)=2;

		if (ShortName) {free(ShortName);ShortName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)malloc(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(3, "b", &n1, &n1, &bOkOutINT);
			LhsVar(2)=3;
			if (bOkOutINT) {free(bOkOutINT);bOkOutINT=NULL;}
		}

		C2F(putlhsvar)();
	
	}
	else 
	{
		Scierror(999,"Not a path string");
		return 0;
	}
	return 0;
	
}
/*-----------------------------------------------------------------------------------*/
/* Allan CORNET INRIA 2005 */
int C2F(inttoolbar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int bOK=FALSE;
	char *Output=NULL;

	CheckRhs(1,2);
	CheckLhs(0,1);

	Output=(char*)malloc(4*sizeof(char));

	if (Rhs==1)
	{
		if ( GetType(1) == sci_matrix )
		{
			int numwin=-2;
			GetRhsVar(1,"i",&m1,&n1,&l1);

			numwin=*istk(l1);

			#ifdef WIN32
			if (GetStateToolBarWin32(numwin))
			{
				strcpy(Output,"on");
			}
			else
			{
				strcpy(Output,"off");
			}
			#else
			strcpy(Output,"off");
			#endif
		}
		else
		{
			Scierror(999,"parameter type incorrect");
			return 0;
		}
	}
	else /*Rhs == 2 */
	{
		if ( (GetType(1) == sci_matrix) && (GetType(2) == sci_strings) )
		{
			int numwin=-2;
			char *param=NULL;

			GetRhsVar(1,"i",&m1,&n1,&l1);
			numwin=*istk(l1);

			GetRhsVar(2,"c",&m1,&n1,&l1);
			param=cstk(l1);

			if ( (strcmp(param,"off")==0) || (strcmp(param,"on")==0) )
			{
				#ifdef WIN32
					if (ToolBarWin32(numwin,param))
					{
						strcpy(Output,"on");
					}
					else
					{
						strcpy(Output,"off");
					}
				#else			
					strcpy(Output,"off");
				#endif
			}
			else
			{
				Scierror(999,"Second parameter incorrect: 'on' or 'off'");
				return 0;
			}
		}
		else
		{
			Scierror(999,"parameter(s) type incorrect");
			return 0;
		}
	}

	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = 1;
	C2F(putlhsvar)();	
	if (Output) {free(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intbanner) _PARAMS((char *fname))
{
	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	banner();

	LhsVar(1) = 0;
	C2F(putlhsvar)();	
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intconsole) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	char *Output=NULL;

	Rhs=Max(Rhs,0);
	CheckRhs(0,1);
	CheckLhs(0,1);

	Output=(char*)malloc(4*sizeof(char));

	if (Rhs==0)
	{
		#ifdef WIN32
		if (IsWindowInterface()) 
			{
				if (GetConsoleState()) strcpy(Output,"on");
				else strcpy(Output,"off");
					
			}
			else
			{
				sciprint("Only on Windows Mode, not in Console Mode.\n");
				strcpy(Output,"off");
			}
		#else
			strcpy(Output,"off");
		#endif
	}
	else
	if (GetType(1) == sci_strings)
		{
			char *param=NULL;

			GetRhsVar(1,"c",&m1,&n1,&l1);
			param=cstk(l1);

			if ( (strcmp(param,"off")==0) || (strcmp(param,"on")==0) )
			{
				#ifdef WIN32
				if (IsWindowInterface()) 
					{
						if (strcmp(param,"on")==0)
							{
								SetConsoleState(1);
								ShowScilex();
								strcpy(Output,"on");
							}
						else
							{
								SetConsoleState(0);
								HideScilex();
								strcpy(Output,"off");
							}
					}
				else
					{
						sciprint("Only on Windows Mode, not in Console Mode.\n");
						strcpy(Output,"off");
					}
				#else			
					strcpy(Output,"off");
				#endif
			}
			else
			{
				Scierror(999,"parameter incorrect: 'on' or 'off'");
				return 0;
			}
		}
		else
		{
			Scierror(999,"parameter incorrect: 'on' or 'off'");
			return 0;
		}

	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = 1;
	C2F(putlhsvar)();	
	if (Output) {free(Output);Output=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/