/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "getstaticdebuginfo.h"
#include "gw_core.h"
#include "stack-c.h"
#include "version.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/

#define NB_DEBUG_ELEMENT 100
int C2F(sci_getdebuginfo) _PARAMS((char *fname,unsigned long fname_len))
{

	CheckRhs(0,0);
	CheckLhs(0,2);
	static int n1=1,m1,n2=1,m2;

	char **outputStaticList=getStaticDebugInfo(&m1);
	char **outputDynamicList=getDynamicDebugInfo(&m2);
 

//	m1=i;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, outputStaticList);
	LhsVar(1) = Rhs+1;
	CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE, &m2, &n2, outputDynamicList);
	LhsVar(2) = Rhs+2;
	C2F(putlhsvar)();
return 0;
	/*
	  C2F(getcomp)( C2F(cha1).buf,&nbuf,128);
	  Str[n1]=C2F(cha1).buf;
	  n1++;

	  C2F(withpvm)(&irep);
	  if (irep)
	  {
	  Str[n1]=pvm;
	  n1++;
	  }

	  C2F(withtk)(&irep);
	  if (irep)
	  {
	  Str[n1]=tk;
	  n1++;
	  }

	  C2F(withmodelicac)(&irep);
	  if (irep)
	  {
	  Str[n1]=modelicac;
	  n1++;
	  }

	  C2F(withjavasci)(&irep);
	  if (irep)
	  {
	  Str[n1]=javasci;
	  n1++;
	  }

	  #ifdef WITH_ATLAS
	  {
	  Str[n1]=atlas;
	  n1++;
	  }
	  #endif

	  #ifdef _MSC_VER
	  #ifdef _DEBUG
	  {
	  Str[n1]=debugmode;
	  n1++;
	  }
	  #else
	  {
	  Str[n1]=releasemode;
	  n1++;
	  }
	  #endif
	  #else
	  #ifdef NDEBUG
	  {
	  Str[n1]=releasemode;
	  n1++;
	  }
	  #else
	  {
	  Str[n1]=debugmode;
	  n1++;
	  }
	  #endif
	  #endif


	  #endif
	*/
}
/*--------------------------------------------------------------------------*/
