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
static int GetFunTabSize(void);
static void DispInternalFunctions(void);
static void DispCommands(void);
static char **CreateStringsInternalFunctions(int MaxStrings);
/*-----------------------------------------------------------------------------------*/
int C2F(intwhat) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int i=0;
	int SizeTabFun=0;

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);
	
	SizeTabFun=GetFunTabSize();

	if (Lhs == 1)
	{
		DispInternalFunctions();
		DispCommands();
		LhsVar(1)=0;
	}
	else /* Lhs == 2 */
	{
		int i=0,j=0;
		int nrowFunctions=SizeTabFun;
		int nrowCommands=nbrCommands;
		
		int ncol=1;
		char **StringFunctions=NULL;

		StringFunctions=CreateStringsInternalFunctions(SizeTabFun);

		CreateVarFromPtr(Rhs+1, "S", &nrowFunctions, &ncol, StringFunctions);
		LhsVar(1)=Rhs+1;

		CreateVarFromPtr(Rhs+2, "S", &nrowCommands, &ncol, CommandWords);
		LhsVar(2)=Rhs+2;

		for (i=0;i<nrowFunctions;i++) { FREE(StringFunctions[i]);StringFunctions[i]=NULL; }
		FREE(StringFunctions);
	}
		
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int GetFunTabSize(void)
{
	int Size=0;
	while ( SciFuncs[Size].name != (char *) 0 ) Size++;
	return (Size);
}
/*-----------------------------------------------------------------------------------*/
static void DispInternalFunctions(void)
{
	int i=0;
	int SizeTabFun=GetFunTabSize();
	sciprint("\n Internal Functions: \n\n");
	for (i=1;i<SizeTabFun+1;i++)
	{
		sciprint("%s\t",SciFuncs[i-1].name);
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
static char **CreateStringsInternalFunctions(int MaxStrings)
{
	char **StringsFunctions=NULL;
	int i=0;

	StringsFunctions=(char **)MALLOC(MaxStrings*sizeof(char **));
	
	for(i=0;i<MaxStrings;i++)
	{
		StringsFunctions[i]=(char*)MALLOC((strlen(SciFuncs[i].name)+1)*sizeof(char));
		sprintf(StringsFunctions[i],"%s",SciFuncs[i].name);
	}

	return (char **)StringsFunctions;
}
/*-----------------------------------------------------------------------------------*/

