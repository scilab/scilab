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
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h"  /* malloc */
#else
#include "../os_specific/sci_mem_alloc.h"  /* malloc */
#endif


#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
#define NULL 0
#endif

#define TRUE  1
#define FALSE 0


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
extern BOOL IsToThePrompt(void);
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

			  p = (CommandRec *) MALLOC (sizeof (CommandRec));
			  if (p == (CommandRec *) 0)
				{
					sciprint ("send_command : No more memory \r\n");
					return (1);
				}
			  p->flag = 0;
			  p->command = (char *) MALLOC ((strlen (command) + 1) * sizeof (char));
			  if (p->command == (char *) 0)
				{
				  FREE(p);
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
      FREE (p->command);
      FREE (p);
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
int C2F(intsleep) _PARAMS((char *fname))
{
	integer m1,n1,l1,sec=0;

	CheckLhs(0,1);
	CheckRhs(1,1);
	if (Rhs == 1) 
	{ 
		GetRhsVar(1,"d",&m1,&n1,&l1);
		CheckScalar(1,m1,n1);
		sec = (integer) *stk(l1);
		if (sec <=0)
		{
			Scierror(999,"sleep: error time must be >0.\r\n");
			return 0;
		}
	
	#ifdef WIN32
		{
			int ms = (sec); /** time is specified in milliseconds in scilab**/
			if (ms > 0) Sleep(ms); /* Number of milliseconds to sleep. */
		}
	#else
		{
			unsigned useconds;
			useconds=(unsigned) sec;
			if (useconds != 0)  
				#ifdef HAVE_USLEEP
					{ usleep(useconds*1000); }
				#else
					#ifdef HAVE_SLEEP
					{  sleep(useconds*1000); }
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

	
	output=(char*)MALLOC((strlen(OperatinSystem)+1)*sizeof(char));
	sprintf(output,"%s",OperatinSystem);
	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(output), &m1),&n1,&output);
	if (output) {FREE(output);output=NULL;}
	LhsVar(1)=1;

	if (Lhs==2)
	{
		char *output2=NULL;
		output2=(char*)MALLOC((strlen(Release)+1)*sizeof(char));
		sprintf(output2,"%s",Release);
		n1=1;
		CreateVarFromPtr( 2, "c",(m1=(int)strlen(output2), &m1),&n1,&output2);
		if (output2) {FREE(output2);output2=NULL;}
		LhsVar(2)=2;
	}
	
	C2F(putlhsvar)();
	
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
