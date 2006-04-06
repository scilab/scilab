/*-----------------------------------------------------------------------------------*/ 
/* INRIA Scilab */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>

#ifdef WIN32
	#include <windows.h>
	#pragma comment(lib, "winmm.lib")
#endif

#include "../stack-c.h"
#include "sox.h" 
#include "stdio.h" 
#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(cluni0) __PARAMS((char *name, char *nams, integer *ln, long int name_len,long int nams_len));  
/*-----------------------------------------------------------------------------------*/ 
/* FILENAME_MAX is set to 14 on hp */
#ifdef hppa 
#undef FILENAME_MAX
#define FILENAME_MAX 4096 
#endif 
/*-----------------------------------------------------------------------------------*/ 
static int BeepON=1;

static char filename[FILENAME_MAX];
static int out_n;
static long int lin,lout;
/*-----------------------------------------------------------------------------------*/ 
/* SCILAB function : savewave */
/*-----------------------------------------------------------------------------------*/ 
int intssavewave(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,mn2,l2,m3,n3,l3,l4,err,rate=22050,channels;
  int un=1;
  Nbvars=0;
  CheckRhs(2,3);
  CheckLhs(1,1);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*  checking variable res */
  GetRhsVar(2,"d",&m2,&n2,&l2);
  mn2=m2*n2;
  /* Checking variable rate */
  if ( Rhs == 3 )
  {
     GetRhsVar(3,"d",&m3,&n3,&l3);
     rate =(int)(*stk(l3));
  }
  CreateVar(Rhs+1, "d", &un,&un, &l4);
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  channels = m2;
  C2F(savewave)(filename,stk(l2),&rate,&mn2,&channels,&err);
  if (err >  0)
	{
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  *stk(l4) = *stk(l2);
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
 /* SCILAB function : loadwave */
 /*-----------------------------------------------------------------------------------*/ 
int intsloadwave(char *fname,unsigned long fname_len)
{
  WavInfo Wi;
  int m1,n1,l1,m2=1,n2,nn2,l2,err=0,un=1,eight=8,l3,m4,n4,l4;
  Nbvars=0;
  CheckRhs(1,1);
  CheckLhs(1,3);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  /*** first call to get the size **/
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  C2F(loadwave)(filename,(double *) 0,&n2,0,&Wi,&err);
  if (err >  0)
	{
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  /* using channels */
  m2 =Wi.wChannels;
  nn2 = n2/m2;
  CreateVar(2,"d",&m2,&nn2,&l2);
  CreateVar(3,"d",&un,&eight,&l3);
  
  *stk(l3)   = Wi.wFormatTag;	/* data format */
  *stk(l3+1) = Wi.wChannels;	/* number of channels */
  *stk(l3+2) = Wi.wSamplesPerSecond; /* samples per second per channel */
  *stk(l3+3) = Wi.wAvgBytesPerSec; /* estimate of bytes per second needed */
  *stk(l3+4) = Wi.wBlockAlign;	/* byte alignment of a basic sample block */
  *stk(l3+5) = Wi.wBitsPerSample; /* bits per sample */
  *stk(l3+6) = Wi.data_length;	/* length of sound data in bytes */
  *stk(l3+7) = Wi.bytespersample; /* bytes per sample (per channel) */

  n4=1;m4=strlen(Wi.wav_format);
  CreateVar(4,"c",&m4,&n4,&l4);
  strncpy(cstk(l4),Wi.wav_format,strlen(Wi.wav_format));
  C2F(loadwave)(filename,stk(l2),&n2,1,&Wi,&err);
  if (err >  0)
	{
    /*sciprint("%s: Internal Error \r\n",fname);*/
    Error(10000);
    return 0;
  };
  LhsVar(1)= 2;
  LhsVar(2)= 3;
  LhsVar(3)= 4;
  PutLhsVar();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
/* Play Sound for windows */
/* Allan CORNET 18/01/2004 */
/*-----------------------------------------------------------------------------------*/ 
int C2F(playsound)(char *fname,char *command,unsigned long fname_len)
{

#ifdef WIN32
  /* Stop Playing*/
  PlaySound(NULL,NULL,SND_PURGE);	
  /* Play Wav file	*/
  PlaySound(filename,NULL,SND_ASYNC|SND_FILENAME);
  return 0;
#else 
  /* linux : a player should be detected by configure ?
   */
  char system_cmd[FILENAME_MAX+10];
  int rep ;
  sprintf(system_cmd,"%s  %s",(command == NULL) ? "play": command, filename);
  rep = system(system_cmd);
  return rep;
#endif 
}
/*-----------------------------------------------------------------------------------*/ 
/* SCILAB function : PlaySound */
/* Allan CORNET 18/01/2004 */
 /*-----------------------------------------------------------------------------------*/ 
int intPlaysound (char *fname,unsigned long fname_len)
{
  char *command=NULL;
  int m1,n1,l1,un=1,rep,m2,n2,l2;
  CheckRhs(1,2);
  CheckLhs(0,1);
  /*  checking variable file */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if ( Rhs == 2 ) 
  {
      GetRhsVar(2,"c",&m2,&n2,&l2);
      command = cstk(l2);
  }
  /*** first call to get the size **/
  lout=FILENAME_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  rep = C2F(playsound)(filename,command,strlen(filename));
  if ( Lhs == 1 ) 
  {
      CreateVar(2,"d",&un,&un,&l2);
      *stk(l2)= rep;
      LhsVar(1)=2;
  }
  else
  {
    if ( rep == -1 ) 
		{
			Scierror(999,"Error in PlaySound\r\n");
		}
    LhsVar(1)=0;
  }
  PutLhsVar();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
void BeepLinuxWindows(void)
{
	#if WIN32
		MessageBeep(-1);
	#else
		system("echo -en \"\a\"");
	#endif
}
/*-----------------------------------------------------------------------------------*/ 
 /* SCILAB function : beep */
/* Allan CORNET Aout 2004 */
 /*-----------------------------------------------------------------------------------*/ 
 int intBeep (char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;
	char *output=NULL ;

	Rhs=Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 1)
	{
		char *param1=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		param1=cstk(l1);

		if ( strcmp(param1,"on") == 0 )
		{
			BeepON=1;
		}
		else 
		{
			if ( strcmp(param1,"off") == 0 )
			{
				BeepON=0;
			}
			else Scierror(999,"Unknown command option.\r\n");
		}
	}
	else
	{
		if (BeepON) BeepLinuxWindows();
	}

	output=(char*)MALLOC(6*sizeof(char));
	n1=1;

	if (BeepON) strcpy(output,"on");
	else strcpy(output,"off");

	CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
	FREE(output);
	output=NULL;

	LhsVar(1) = 1;

	PutLhsVar();
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
