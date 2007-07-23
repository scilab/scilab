/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "stack-c.h"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
#define MSG_WERROR1 "parameter(s) type incorrect.\r\n"
#define MSG_WERROR2 "Error.\r\n"
#define MSG_WERROR3 "Cannot query value of this type.\r\n"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
BOOL WindowsQueryRegistry(char *ParamIn1,char *ParamIn2,char *ParamIn3,char *ParamOut1,int *ParamOut2,BOOL *OuputIsREG_SZ);
BOOL WindowsQueryRegistryList(char *ParamIn1,char *ParamIn2,int dimMax,char **ListKeys);
BOOL WindowsQueryRegistryNumberOfElementsInList(char *ParamIn1,char *ParamIn2,int *Number);
#endif
/*-----------------------------------------------------------------------------------*/
int InterfaceWindowsQueryRegistry _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_winqueryreg) _PARAMS((char *fname,unsigned long l))
{
#ifdef _MSC_VER
	InterfaceWindowsQueryRegistry(fname);
#else
	Scierror(999,"Only for Windows\r\n");
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int InterfaceWindowsQueryRegistry _PARAMS((char *fname))
{
#ifdef _MSC_VER
	static int l1,n1,m1;

	char *param1=NULL,*param2=NULL,*param3=NULL;
	char *output=NULL ;
	int *paramoutINT=NULL;
	BOOL OuputIsREG_SZ=FALSE;
	BOOL TestWinQuery=FALSE;

	Rhs=Max(0,Rhs);
	CheckRhs(2,3);
	CheckLhs(0,1);

	if (Rhs == 3)
	{
		if ( (GetType(1) != sci_strings) || (GetType(2) != sci_strings) || (GetType(3) != sci_strings))
		{
			Scierror(999,MSG_WERROR1);
			return 0;
		}
	}
	else /* Rhs == 2 */
	{
		if ( (GetType(1) != sci_strings) || (GetType(2) != sci_strings) )
		{
			Scierror(999,MSG_WERROR1);
			return 0;
		}
	}

	GetRhsVar(1,"c",&m1,&n1,&l1);
	param1=cstk(l1);

	GetRhsVar(2,"c",&m1,&n1,&l1);
	param2=cstk(l1);

	if ( Rhs == 3 )
	{
		GetRhsVar(3,"c",&m1,&n1,&l1);
		param3=cstk(l1);

		if (strcmp(param1,"name") == 0)
		{
			int NumbersElm=0;

			WindowsQueryRegistryNumberOfElementsInList(param2,param3,&NumbersElm);
			if (NumbersElm)
			{
				static char *ListKeysName[255];

				if (NumbersElm > 255) NumbersElm=255;
				if ( WindowsQueryRegistryList(param2,param3,NumbersElm,ListKeysName) )
				{
					int i=0;

					CreateVarFromPtr( Rhs+1, "S", &NumbersElm, &n1, &ListKeysName);

					for (i=0; i<NumbersElm;i++)
					{
						FREE(ListKeysName[i]);
						ListKeysName[i]=NULL;
					}

					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
					return 0;
				}
				else
				{
					Scierror(999,MSG_WERROR2);
					return 0;
				}
			}
			else
			{
				m1=0;
				n1=0;
				l1=0;
				CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
				LhsVar(1)=Rhs+1;
				C2F(putlhsvar)();
				return 0;
			}
			return 0;
		}
	}

	output=(char*)MALLOC(PATH_MAX*sizeof(char));
	paramoutINT=(int*)MALLOC(sizeof(int));

	if ( Rhs == 3 )
	{
		TestWinQuery=WindowsQueryRegistry(param1,param2,param3,output,paramoutINT,&OuputIsREG_SZ);
	}
	else
	{
		TestWinQuery=WindowsQueryRegistry(param1,param2,NULL,output,paramoutINT,&OuputIsREG_SZ);
	}

	if ( TestWinQuery )
	{
		n1=1;
		if ( OuputIsREG_SZ )
		{
			CreateVarFromPtr(Rhs+1, "c",(m1=(int)strlen(output), &m1),&n1,&output);
		}
		else
		{
			CreateVarFromPtr(Rhs+1, "i", &n1, &n1, &paramoutINT);
		}

		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		if (output) {FREE(output);output=NULL;}
		if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
		Scierror(999,MSG_WERROR3);
		return 0;
	}

	if (output) {FREE(output);output=NULL;}
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
