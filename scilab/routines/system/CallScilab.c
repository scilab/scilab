/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "CallScilab.h"
/*-----------------------------------------------------------------------------------*/
static char DefaultScilabStartup[]="SCI/scilab.star";
static char DefaultScilabQuit[]="SCI/scilab.quit";
static char DefaultSCIenv[]="../..";
static int DefaultStacksize=1000000;
static int StartScilabIsOK=FALSE;
/*-----------------------------------------------------------------------------------*/
extern int setenvc(char *string,char *value);
extern int C2F(inisci)(int *,int *,int *);
extern void C2F(settmpdir) (void);
extern int C2F(scirun)(char * startup, int lstartup);
extern void C2F (tmpdirc) (void);
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32

extern void initTCLTK(void);

static void SetSciEnv(void)
{
  extern void set_sci_env(char *DefaultSCIPATH);

  #define MAXSTR 4096
  LPSTR tail;
  int i=0;
  char szModuleName[MAXSTR];
  char SCIPATH[MAXSTR];
  char WSCIPATH[MAXSTR];

  /* Get full name of this program */
  /* ex : c:\Program Files\scilab-3.0\bin\prog.exe */  
  GetModuleFileName ((HANDLE)GetModuleHandle(NULL),  (LPSTR) szModuleName, MAXSTR);
  
  /* remove prog.exe from szModuleName */ 	
  /* szModuleName --> c:\Program Files\scilab-3.0\bin\ */  	
  if ((tail = strrchr (szModuleName, '\\')) != (LPSTR) NULL)
  {
	tail++;
	*tail = '\0';
  }
  
  szModuleName[strlen(szModuleName)-1]='\0';
  /* copy szModuleName in WSCIPATH */
  wsprintf(WSCIPATH,"%s",szModuleName);
  /* remove bin in WSCIPATH */
  /* WSCIPATH --> c:\Program Files\scilab-3.0\ */  		
  if ((tail = strrchr (WSCIPATH, '\\')) != (LPSTR) NULL)
  {
	tail++;
	*tail = '\0';
  }
  
  /* copy WSCIPATH in SCIPATH */
  wsprintf(SCIPATH,"%s",WSCIPATH);
	
  /* convert \ on / in SCIPATH*/	
  for (i=0;i<(int)strlen(SCIPATH);i++)
  {
	if (SCIPATH[i]=='\\') SCIPATH[i]='/';
  }
	
  /* WSCIPATH --> c:\Program Files\scilab-3.0 */  			
  /* SCIPATH -->  c:/Program Files/scilab-3.0 */  			
  WSCIPATH[strlen(WSCIPATH)-1]='\0';
  SCIPATH[strlen(SCIPATH)-1]='\0';
	
  /* set scilab variables environment */
  set_sci_env(SCIPATH);
}
#endif
/*-----------------------------------------------------------------------------------*/
int StartScilab(char *SCIpath,char *ScilabStartup,int *Stacksize)
{
	int bOK=FALSE;

	char *SCIpathUsed=NULL;
	char *ScilabStartupUsed=NULL;
	char *InitStringToScilab=NULL;
	int StacksizeUsed=0;
	int lengthStringToScilab=0;

	static int iflag=-1,ierr=0;

	if (StartScilabIsOK) return bOK;

	if (SCIpath==NULL)
	{
		#ifdef WIN32
			SetSciEnv();
		#else
		{
			char env[2048];
			sprintf(env,"SCI=%s",SCIpath);
			putenv(env);
		}
		#endif
	}
	else
	{
		char env[2048];
		sprintf(env,"SCI=%s",SCIpath);
		putenv(env);
	}

	if (ScilabStartup==NULL)
	{
		ScilabStartupUsed=(char*)malloc((strlen(DefaultScilabStartup)+1)*sizeof(char));
		sprintf(ScilabStartupUsed,"%s",DefaultScilabStartup);
	}
	else
	{
		ScilabStartupUsed=(char*)malloc((strlen(DefaultScilabStartup)+1)*sizeof(char));
		sprintf(ScilabStartupUsed,"%s",ScilabStartup);
	}
	
	if (Stacksize==NULL)
	{
		StacksizeUsed=DefaultStacksize;
	}
	else
	{
		StacksizeUsed=*Stacksize;
	}

	/* Scilab Initialization */ 
	#ifdef WITH_TK
	  initTCLTK(); /* TCLTK Init. */
    #endif
	C2F(inisci)(&iflag,&StacksizeUsed,&ierr);

	if ( ierr > 0 ) 
    {
	  bOK=FALSE;
      return bOK;
    }

	/* running the startup */ 
	C2F(settmpdir)();
	lengthStringToScilab=(int)(strlen("exec(\"SCI/scilab.star\",-1);quit;")+strlen(ScilabStartupUsed));
	InitStringToScilab=(char*)malloc(lengthStringToScilab*sizeof(char));
	sprintf(InitStringToScilab,"exec(\"%s\",-1);quit;",ScilabStartupUsed);

	
	C2F(scirun)(InitStringToScilab,strlen(InitStringToScilab));

	if (ScilabStartupUsed) {free(ScilabStartupUsed);ScilabStartupUsed=NULL;}
	if (InitStringToScilab) {free(InitStringToScilab);InitStringToScilab=NULL;}

	bOK=TRUE;
	StartScilabIsOK=TRUE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int TerminateScilab(char *ScilabQuit)
{
	int bOK=FALSE;

	if (StartScilabIsOK)
	{
		char *ScilabQuitUsed=NULL;
		char *QuitStringToScilab=NULL;
		int lengthStringToScilab=0;

		if (ScilabQuit==NULL)
		{
			ScilabQuitUsed=(char*)malloc((strlen(DefaultScilabQuit)+1)*sizeof(char));
			sprintf(ScilabQuitUsed,"%s",DefaultScilabQuit);
		}
		else
		{
			ScilabQuitUsed=(char*)malloc((strlen(ScilabQuit)+1)*sizeof(char));
			sprintf(ScilabQuitUsed,"%s",ScilabQuit);
		}

		lengthStringToScilab=(int)(strlen("exec('SCI/scilab.quit',-1);quit;")+strlen(ScilabQuitUsed));
		QuitStringToScilab=(char*)malloc(lengthStringToScilab*sizeof(char));
		sprintf(QuitStringToScilab,"exec(\"%s\",-1);quit;",ScilabQuitUsed);

		C2F(tmpdirc)();

		if (QuitStringToScilab) {free(QuitStringToScilab);QuitStringToScilab=NULL;}
		if (ScilabQuitUsed) {free(ScilabQuitUsed);ScilabQuitUsed=NULL;}

		StartScilabIsOK=FALSE;
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SendScilabJob(char *job)
{
  int m,n,lp;
  static char buf[1024];
  char format[]="Err=execstr('%s','errcatch','n');quit;";
  
  sprintf(buf,format,job);
  C2F(scirun)(buf,strlen(buf));
  GetMatrixptr("Err", &m, &n, &lp);

  return (int) *stk(lp);
}
/*-----------------------------------------------------------------------------------*/