/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "CallScilab.h"
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

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
extern int C2F(ismenu)();
extern void SetFromCToON(void);
extern int version_flag(void); 
extern void sciGetIdFigure (int *vect, int *id, int *flag);
extern int IsFromC(void);
extern void C2F(freegmem)(void);
extern void C2F(freemem)(void);
#ifdef WITH_TK
extern int CloseTCLsci(void);
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
extern char *GetScilabDirectory(BOOL UnixStyle);
extern void initTCLTK(void);
extern void start_sci_gtk();
extern void InitWindowGraphDll(void);
extern void TextMessage1 (int ctrlflag);
#else
extern int C2F(sxevents)();
#endif
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
static void SetSciEnv(void)
{
  extern void set_sci_env(char *DefaultSCIPATH);
  
  char *ScilabDirectory=NULL;

  ScilabDirectory=GetScilabDirectory(TRUE);

  if (ScilabDirectory == NULL)
  {
	MessageBox (NULL, "Error", "GetScilabDirectory()", MB_ICONSTOP | MB_OK);
	exit(1);
  }
  set_sci_env(ScilabDirectory);

  if (ScilabDirectory){FREE(ScilabDirectory);ScilabDirectory=NULL;}		
  
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
		ScilabStartupUsed=(char*)MALLOC((strlen(DefaultScilabStartup)+1)*sizeof(char));
		sprintf(ScilabStartupUsed,"%s",DefaultScilabStartup);
	}
	else
	{
		ScilabStartupUsed=(char*)MALLOC((strlen(DefaultScilabStartup)+1)*sizeof(char));
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

	SetFromCToON();

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
#if WIN32
	InitWindowGraphDll();
	start_sci_gtk() ;
#endif

	lengthStringToScilab=(int)(strlen("exec(\"SCI/scilab.star\",-1);quit;")+strlen(ScilabStartupUsed));
	InitStringToScilab=(char*)MALLOC(lengthStringToScilab*sizeof(char));
	sprintf(InitStringToScilab,"exec(\"%s\",-1);quit;",ScilabStartupUsed);

	
	C2F(scirun)(InitStringToScilab,strlen(InitStringToScilab));

	if (ScilabStartupUsed) {FREE(ScilabStartupUsed);ScilabStartupUsed=NULL;}
	if (InitStringToScilab) {FREE(InitStringToScilab);InitStringToScilab=NULL;}

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
			ScilabQuitUsed=(char*)MALLOC((strlen(DefaultScilabQuit)+1)*sizeof(char));
			sprintf(ScilabQuitUsed,"%s",DefaultScilabQuit);
		}
		else
		{
			ScilabQuitUsed=(char*)MALLOC((strlen(ScilabQuit)+1)*sizeof(char));
			sprintf(ScilabQuitUsed,"%s",ScilabQuit);
		}

		lengthStringToScilab=(int)(strlen("exec('SCI/scilab.quit',-1);quit;")+strlen(ScilabQuitUsed));

		QuitStringToScilab=(char*)MALLOC(lengthStringToScilab*sizeof(char));
		sprintf(QuitStringToScilab,"exec(\"%s\",-1);quit;",ScilabQuitUsed);

		/* C2F(scirun)(QuitStringToScilab,strlen(QuitStringToScilab)); */

		if (QuitStringToScilab) {FREE(QuitStringToScilab);QuitStringToScilab=NULL;}
		if (ScilabQuitUsed) {FREE(ScilabQuitUsed);ScilabQuitUsed=NULL;}

		#if WIN32
			C2F(freegmem)();
			C2F(freemem)();
		#endif

		#ifdef WITH_TK
			CloseTCLsci();
		#endif

		StartScilabIsOK=FALSE;
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int SendScilabJob(char *job)
{
  int m,n,lp;
  double code=-1;

  static char buf[1024];
  char format[]="Err=execstr('%s','errcatch','n');quit;";
  
  if ((strlen(format)+strlen(job))>= 1024)
  {
	fprintf(stderr,"Error : SendScilabJob string 'job' too long \n");
	return (int) code;
  }

  sprintf(buf,format,job);

  C2F(scirun)(buf,strlen(buf));

  if ( ! C2F(cmatptr)("Err", &m, &n, &lp,strlen("Err")))
  {
	  fprintf(stderr,"Error : SendScilabJob (cmatptr) 'Err' \n");
  }
  else
  {
	  if (m*n == 1)
	  {
		ReadMatrix("Err", &m, &n, &code);
	  }
	  else fprintf(stderr,"Error : SendScilabJob (ReadMatrix Err)\n");
  }

  return (int) code;
}
/*-----------------------------------------------------------------------------------*/
void ScilabDoOneEvent(void)
{
#if WIN32
	TextMessage1 (1);
#else 
	C2F(sxevents)();
#endif

	while(C2F(ismenu)()==1 ) 
	{
		C2F(scirun)("quit;",(int)strlen("quit;"));
	}
}
/*-----------------------------------------------------------------------------------*/
int ScilabHaveAGraph(void)
{
	integer iflag =0,ids,num;
	int vInt=0;

	if (version_flag() == 0)
	{
		sciGetIdFigure (&ids,&num,&iflag);
		if (num > 0) vInt=1;

	}/* NG end*/
	else
	{
		C2F(getwins)(&num,&ids ,&iflag);
		if (num > 0) vInt=1;
	} 

	return vInt;
}
/*-----------------------------------------------------------------------------------*/
