/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_wintools.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern int IsAScalar(int RhsNumber);
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	extern BOOL SetIhmSystemDefaultTextBackgroundColor(void);
	extern BOOL SetIhmSystemDefaultTextColor(void);
	extern BOOL SetIhmTextBackgroundColor(int R,int G,int B);
	extern BOOL SetIhmTextColor(int R,int G,int B);
	extern BOOL IsWindowInterface(void);
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_settextcolor) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	Rhs=Max(Rhs,0);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		#ifdef _MSC_VER
		if (IsWindowInterface()) *paramoutINT=(int)SetIhmSystemDefaultTextColor();
		else *paramoutINT=(int)FALSE;
		#else
		*paramoutINT=(int)FALSE;
		#endif
	}
	else
	{
		if (Rhs == 3)
		{
			if ( IsAScalar(1) && IsAScalar(2) && IsAScalar(3) )
			{
				int R=-1;
				int G=-1;
				int B=-1;

				GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				R=*istk(l1);

				GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				G=*istk(l1);

				GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				B=*istk(l1);

				if ((R>=0) && (R<256) &&
					(G>=0) && (G<256) &&
					(B>=0) && (B<256) )
				{
					#ifdef _MSC_VER
					if (IsWindowInterface()) *paramoutINT=(int)SetIhmTextColor(R,G,B);
					else *paramoutINT=(int)FALSE;
					#else
					*paramoutINT=(int)FALSE;
					#endif
				}
				else
				{
					Scierror(999,"R,G,B >=0 and <256.\n");
					return 0;
				}
			}
			else
			{
				Scierror(999,"Parameter(s) incorrect type.\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"Parameter incorrect type.\n");
			return 0;
		}
	}

	n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1,&paramoutINT);

	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	FREE(paramoutINT);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(sci_settextbackgroundcolor) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	Rhs=Max(Rhs,0);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		#ifdef _MSC_VER
		if (IsWindowInterface()) *paramoutINT=(int)SetIhmSystemDefaultTextBackgroundColor();
		else *paramoutINT=(int)FALSE;
		#else
		*paramoutINT=(int)FALSE;
		#endif
	}
	else
	{
		if (Rhs == 3)
		{
			if ( IsAScalar(1) && IsAScalar(2) && IsAScalar(3) )
			{
				int R=-1;
				int G=-1;
				int B=-1;

				GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				R=*istk(l1);

				GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				G=*istk(l1);

				GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				B=*istk(l1);
				if ( (R>=0) && (R<256) &&
					 (G>=0) && (G<256) &&
					 (B>=0) && (B<256) )
				{
					#ifdef _MSC_VER
					if (IsWindowInterface()) *paramoutINT=(int)SetIhmTextBackgroundColor(R,G,B);
					else *paramoutINT=(int)FALSE;
					#else
					*paramoutINT=(int)FALSE;
					#endif
				}
				else
				{
					Scierror(999,"R,G,B >=0 and <256.\n");
					return 0;
				}
			}
			else
			{
				Scierror(999,"Parameter(s) incorrect type.\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"Parameter incorrect type.\n");
			return 0;
		}
	}

	n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1,&paramoutINT);

	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	FREE(paramoutINT);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
