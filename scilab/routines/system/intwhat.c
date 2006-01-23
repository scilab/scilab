/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "intwhat.h"
#ifdef WIN32
  #include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
  #include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/ 
const int nbrCommands=29;
/*-----------------------------------------------------------------------------------*/
static char *CommandWords[]={
	"if","else",
	"for","while",
	"end","select",
	"case","quit",
	"exit","return",
	"help","what",
	"who","pause",
	"clear","resume",
	"then","do",
	"apropos","abort",
	"break","elseif",
	"pwd","function",
	"endfunction","clc",
	"continue",
	"try","catch"
};
static char **LocalFunctionsTab=NULL;
static int SizeLocalFunctionsTab=0;
/*-----------------------------------------------------------------------------------*/
typedef struct 
{
	char *name;
	int codeI;
	int code;
	int level;
} Funcs ;
/*-----------------------------------------------------------------------------------*/
extern Funcs SciFuncs[];
/*-----------------------------------------------------------------------------------*/
static int CreateLocalFunctionsTab(void);
static int GetFunTabSizes(int *MaxSize,int *MaxSizeWithoutCommands);
static int IsACommand(char *primitive);
static void DispInternalFunctions(void);
static void DispCommands(void);
static void SortStrings(char **Strs,int SizeOfStrs);
/*-----------------------------------------------------------------------------------*/
int C2F(intwhat) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int i=0;
	int Size=0;

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

	SortStrings(CommandWords,nbrCommands);
	CreateLocalFunctionsTab();
	SortStrings(LocalFunctionsTab,SizeLocalFunctionsTab);

	
	if (Lhs == 1)
	{
		DispInternalFunctions();
		DispCommands();
		LhsVar(1)=0;
	}
	else /* Lhs == 2 */
	{
		int i=0;

		int ncol=1;	
		int nrowFunctions=SizeLocalFunctionsTab;
		int nrowCommands=nbrCommands;

		CreateVarFromPtr(Rhs+1, "S", &nrowFunctions, &ncol, LocalFunctionsTab);
		LhsVar(1)=Rhs+1;

		CreateVarFromPtr(Rhs+2, "S", &nrowCommands, &ncol, CommandWords);
		LhsVar(2)=Rhs+2;

		for (i=0;i<nrowFunctions;i++) { FREE(LocalFunctionsTab[i]);LocalFunctionsTab[i]=NULL; }
		FREE(LocalFunctionsTab);
	}
		
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int GetFunTabSizes(int *MaxSize,int *MaxSizeWithoutCommands)
{
	int bOK=TRUE;
	int i=0;
	int Size=0;
	while ( SciFuncs[i].name != (char *) 0 )
	{
		if (!IsACommand(SciFuncs[i].name)) Size++;
		i++;
	}

	*MaxSize=i;
	*MaxSizeWithoutCommands=Size;

	return (bOK);
}
/*-----------------------------------------------------------------------------------*/
static void DispInternalFunctions(void)
{
	int i=0;
	
	sciprint("\n Internal Functions: \n\n");
	for (i=1;i<SizeLocalFunctionsTab-1;i++)
	{
		sciprint("%+24s ",LocalFunctionsTab[i-1]);
		if (i%4==0) sciprint("\n");
	}
	sciprint("\n");
}
/*-----------------------------------------------------------------------------------*/
static void DispCommands(void)
{
	int i=0;

	sciprint("\n Commands: \n\n");
	for (i=1;i<nbrCommands+1;i++)
	{
		sciprint("%+24s ",CommandWords[i-1]);
		if (i%4==0) sciprint("\n");
	}
	sciprint("\n");
}
/*-----------------------------------------------------------------------------------*/
static int IsACommand(char *primitive)
{
	int bOK=FALSE;
	int i=0;
	for (i=0;i<nbrCommands;i++)
	{
		if (strcmp(CommandWords[i],primitive)==0)
		{
			return TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
static int CreateLocalFunctionsTab(void)
{
	int SizeTab=0;
	int SizeTabWithoutCommands=0;
	int i=0;
	int j=0;

	GetFunTabSizes(&SizeTab,&SizeTabWithoutCommands);
	SizeLocalFunctionsTab=SizeTabWithoutCommands;
	LocalFunctionsTab=(char **)MALLOC(SizeLocalFunctionsTab*sizeof(char **));
	
	j=0;
	for(i=0;i<SizeTab;i++)
	{
		if (!IsACommand(SciFuncs[i].name))
		{
			LocalFunctionsTab[j]=(char*)MALLOC((strlen(SciFuncs[i].name)+1)*sizeof(char));
			sprintf(LocalFunctionsTab[j],"%s",SciFuncs[i].name);
			j++;
		}
	}
	
	return TRUE;
}
/*-----------------------------------------------------------------------------------*/
static void SortStrings(char **Strs,int SizeOfStrs)
{
	int fin,i;
	for(fin=SizeOfStrs-1;fin>0;fin--)
	{
		int Sorted=FALSE;
		for(i=0;i<fin;i++)
		{
			if(strcmp(Strs[i],Strs[i+1])>0)
			{
				char *tmp;
				memcpy(&tmp,Strs+i,sizeof(char *));
				memcpy(Strs+i,Strs+(i+1),sizeof(char *));
				memcpy(Strs+(i+1),&tmp,sizeof(char *));
				Sorted=TRUE;
			}
		}
		if(!Sorted)break;
	}
}
/*-----------------------------------------------------------------------------------*/
