/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "inttoolbar.h"
/*-----------------------------------------------------------------------------------*/
#ifdef WIN32
  extern int HideToolBarWin32(int WinNum); /* see "wsci/wmenu.c" */
  extern int ToolBarWin32(int WinNum,char *onoff);
  extern int GetStateToolBarWin32(int WinNum);
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(inthidetoolbar) _PARAMS((char *fname))
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
