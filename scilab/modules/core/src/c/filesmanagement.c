/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include "machine.h"
#include "filesmanagement.h"
#include "core_math.h" /* Min Max */
#include "MALLOC.h" 
/*-----------------------------------------------------------------------------------*/
#define DEFAULT_MAX_FILES 20 
/*-----------------------------------------------------------------------------------*/
typedef struct {
	FILE *ftformat;
	int ftswap; /* swap status for each file */
	int ftmode; /* mode for each file */
	int fttype; /* type (Fortran,C) for each file must be zero initialized */
	char *ftname; /* name for each file */
	
} scilabfile;
/*-----------------------------------------------------------------------------------*/
static scilabfile *ScilabFileList = NULL;
static int CurFile =-1;
static int CurrentMaxFiles=DEFAULT_MAX_FILES;
/*-----------------------------------------------------------------------------------*/
FILE *GetFileOpenedInScilab(int Id)
{
	int fd1=0;

	fd1 = (Id != -1) ?  Min(Max(Id,0),GetMaximumFileOpenedInScilab()-1) : CurFile ;

	if ( fd1 != -1 ) return(ScilabFileList[fd1].ftformat);
	return((FILE *) 0);
}
/*-----------------------------------------------------------------------------------*/
int GetCurrentFileId(void)
{
	return CurFile;
}
/*-----------------------------------------------------------------------------------*/
void SetCurrentFileId(int Id)
{
	CurFile=Id;
}
/*-----------------------------------------------------------------------------------*/
void SetFileOpenedInScilab(int Id,FILE *fptr)
{
	ScilabFileList[Id].ftformat=fptr;
}
/*-----------------------------------------------------------------------------------*/
int GetSwapStatus(integer Id)
{
	int fd1;
	fd1 = (Id != -1) ?  Min(Max(Id,0),GetMaximumFileOpenedInScilab()-1) : GetCurrentFileId() ;
	if ( fd1 != -1 ) return(ScilabFileList[fd1].ftswap);
	return(0);
}
/*-----------------------------------------------------------------------------------*/
void SetSwapStatus(int Id,int newswap)
{
	ScilabFileList[Id].ftswap =  newswap;
}
/*-----------------------------------------------------------------------------------*/
int GetMaximumFileOpenedInScilab(void)
{
	return CurrentMaxFiles;
}
/*-----------------------------------------------------------------------------------*/
int GetFileModeOpenedInScilab(int Id)
{
	return ScilabFileList[Id].ftmode;
}
/*-----------------------------------------------------------------------------------*/
void SetFileModeOpenedInScilab(int Id,int mode)
{
	ScilabFileList[Id].ftmode = mode;
}
/*-----------------------------------------------------------------------------------*/
int GetFileTypeOpenedInScilab(int Id)
{
	return ScilabFileList[Id].fttype;
}
/*-----------------------------------------------------------------------------------*/
void SetFileTypeOpenedInScilab(int Id,int Type)
{
	ScilabFileList[Id].fttype = Type;
}
/*-----------------------------------------------------------------------------------*/
char* GetFileNameOpenedInScilab(int Id)
{
	return ScilabFileList[Id].ftname;
}
/*-----------------------------------------------------------------------------------*/
BOOL SetFileNameOpenedInScilab(int Id,char *name)
{
	BOOL bOK=FALSE;
	char *ptrName=NULL;

	ptrName=(char*)MALLOC(sizeof(char)*(strlen(name)+1));
	if (ptrName)
	{
		ScilabFileList[Id].ftname = ptrName;
		strcpy(ptrName,name);
		bOK=TRUE;
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL FreeFileNameOpenedInScilab(int Id)
{
	BOOL bOK=FALSE;
	char *ptr = ScilabFileList[Id].ftname;
	if (ptr) { FREE(ptr);  ptr = NULL; bOK=TRUE;}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL InitializeScilabFilesList(void)
{
	BOOL bOK=FALSE;
	if (!ScilabFileList)
	{
		CurrentMaxFiles=DEFAULT_MAX_FILES;
		ScilabFileList=(scilabfile *)MALLOC(sizeof(scilabfile)*CurrentMaxFiles);

		if (ScilabFileList)
		{
			int i=0;
			for (i=0;i<CurrentMaxFiles;i++)
			{
				ScilabFileList[i].ftformat=NULL;
				ScilabFileList[i].ftmode=0;
				ScilabFileList[i].ftname=NULL;
				ScilabFileList[i].ftswap=0;
				ScilabFileList[i].fttype=0;
			}
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL TerminateScilabFilesList(void)
{
	BOOL bOK=FALSE;

	if (ScilabFileList)
	{
		FREE(ScilabFileList);
		ScilabFileList=NULL;
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
BOOL ExtendScilabFilesList(int NewSize)
{
	BOOL bOK=FALSE;
	if (ScilabFileList)
	{
		if (NewSize > CurrentMaxFiles)
		{
			
			scilabfile *ScilabFileListTmp=NULL;
			ScilabFileListTmp=(scilabfile *)REALLOC(ScilabFileList,NewSize*sizeof(scilabfile));
			if (ScilabFileListTmp)
			{
				int i=0;
				ScilabFileList=ScilabFileListTmp;
				for (i=CurrentMaxFiles;i<NewSize;i++)
				{
					ScilabFileList[i].ftformat=NULL;
					ScilabFileList[i].ftmode=0;
					ScilabFileList[i].ftname=NULL;
					ScilabFileList[i].ftswap=0;
					ScilabFileList[i].fttype=0;
				}
				CurrentMaxFiles=NewSize;
				bOK=TRUE;
			}
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
