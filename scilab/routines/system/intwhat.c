/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef WIN32
  #include "../wsci/win_mem_alloc.h" /* MALLOC */
#else
  #include "../sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/ 
#include "intwhat.h"
/*-----------------------------------------------------------------------------------*/
const int nbrCommands=27;
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
	"continue"  
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
static BOOL CreateLocalFunctionsTab(void);
static BOOL GetFunTabSizes(int *MaxSize,int *MaxSizeWithoutCommands);
static BOOL IsACommand(char *primitive);
static void DispInternalFunctions(void);
static void DispCommands(void);
/*-----------------------------------------------------------------------------------*/
int C2F(intwhat) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int i=0;
	int Size=0;

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

	CreateLocalFunctionsTab();

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
static BOOL GetFunTabSizes(int *MaxSize,int *MaxSizeWithoutCommands)
{
	BOOL bOK=TRUE;
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
		sciprint("%s\t",LocalFunctionsTab[i-1]);
		if (i%5==0) sciprint("\n");
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
		sciprint("%s\t",CommandWords[i-1]);
		if (i%5==0) sciprint("\n");
	}
	sciprint("\n");
}
/*-----------------------------------------------------------------------------------*/
static BOOL IsACommand(char *primitive)
{
	BOOL bOK=FALSE;
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
static BOOL CreateLocalFunctionsTab(void)
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
