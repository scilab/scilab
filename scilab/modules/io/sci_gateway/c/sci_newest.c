/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "gw_io.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "stack-c.h"
#include "cluni0.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int GetIndexLastModifiedFileInList(char **ListFilename,int numberelemnts);
/*--------------------------------------------------------------------------*/
int C2F(sci_newest) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckLhs(1,1);
	if (Rhs < 1)
	{
		/* newest() --> [] */
		int m1,n1,l1;
		m1=0;
		n1=0;
		l1=0;

		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		return 0;
	}
	else
	{
		int m1,n1,l1;
		int RetIndex=1;
		int j=0;

		for (j=1;j<=Rhs;j++)
		{
			if (j == 1)
			{
				/* newest([]) --> [] */
				if (GetType(j) == sci_matrix)
				{
					GetRhsVar(j,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);

					if ( (m1==0) && (n1==0) )
					{
						m1=0;
						n1=0;
						l1=0;

						CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
						LhsVar(1)=Rhs+1;
						C2F(putlhsvar)();
						return 0;
					}
					else
					{
						Scierror(999,_("%s: incorrect %dst parameter.\n"),fname,j);
						return 0;

					}
				}
			}

			if (GetType(j) != sci_strings)
			{
				Scierror(999,_("%s: incorrect %dth parameter(s).\n"),fname,j);
				return 0;
			}
		}

		if (Rhs == 1)
		{
			char **Str=NULL;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

			if ( (m1 != 1) && (n1 != 1) )
			{
				Scierror(999,_("not a vector of filenames.\n"));
				return 0;
			}
			RetIndex=GetIndexLastModifiedFileInList(Str,m1*n1);
		}
		else
		{
			char **Str=NULL;
			int RhsBackup=Rhs;

			Str=(char**)MALLOC(sizeof(char*)*RhsBackup);

			for (j=1;j<=RhsBackup;j++)
			{

				GetRhsVar(j,STRING_DATATYPE,&m1,&n1,&l1);
				Str[j-1]=MALLOC(sizeof(char)*(strlen(cstk(l1))+1));
				strcpy(Str[j-1],cstk(l1));
			}

			RetIndex=GetIndexLastModifiedFileInList(Str,RhsBackup);
			if (Str)
			{
				for (j=1;j<=RhsBackup;j++)
				{
					FREE(Str[j-1]);
					Str[j-1]=NULL;
				}
				FREE(Str);
				Str=NULL;
			}
		}

		if (RetIndex>=1)
		{
			int *paramoutINT=(int*)MALLOC(sizeof(int));
			*paramoutINT = RetIndex;

			n1=1;
			CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n1, &n1, &paramoutINT);
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();

			if (paramoutINT){FREE(paramoutINT); paramoutINT=NULL;}
		}
		else
		{
			m1=0;
			n1=0;
			l1=0;

			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();
		}

	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int GetIndexLastModifiedFileInList(char **ListFilename,int numberelemnts)
{
#ifdef _MSC_VER
	struct _stat buf;
#else
	struct stat buf;
#endif

	int i=0;
	int RetVal=0;

	int RetIndex=1;
	long int MaxTime=0;

	for (i = 0; i<numberelemnts ;i++)
	{

		int resultstat=0;
		char *FileName=NULL;
		int out_n;
		long int lin=(long int)strlen(ListFilename[i]);
		long int lout=(long int)strlen(ListFilename[i])+PATH_MAX;

		FileName=(char*)MALLOC(sizeof(char)*(lout+1));
		C2F(cluni0)(ListFilename[i], FileName, &out_n,lin,lout);
#ifdef _MSC_VER
		if (FileName)
		{
			if ( (FileName[strlen(FileName)-1]=='/') || (FileName[strlen(FileName)-1]=='\\') )
			{
				FileName[strlen(FileName)-1]='\0';
			}

		}
		resultstat = _stat(FileName, &buf );
#else
		resultstat = stat(FileName, &buf );
#endif
		if (resultstat == 0)
		{
			if ((long int)buf.st_mtime>MaxTime)
			{
				MaxTime=(long int)buf.st_mtime;
				RetIndex=i+1;
			}
		}

		FREE(FileName);
		FileName=NULL;
	}

	RetVal=RetIndex;
	return RetVal;
}
/*--------------------------------------------------------------------------*/
