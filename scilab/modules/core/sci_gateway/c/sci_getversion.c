/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdio.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "version.h" 
#include "MALLOC.h"
#include "with_module.h"
#include "loadversion.h"
#include "inisci-c.h"
#include "error_scilab.h"
#include "../../../tclsci/includes/withtk.h"
/*-----------------------------------------------------------------------------------*/
static int getversion_no_rhs(void);
static int getversion_one_rhs(void);
static int getversion_two_rhs(void);
/*-----------------------------------------------------------------------------------*/
#define VERSION_STRING "string_info" 
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getversion) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs=Max(0,Rhs);

	CheckRhs(0,2);
	CheckLhs(1,2);

	if ( Rhs == 0 )
	{
		getversion_no_rhs();
	}
	else if ( Rhs == 1)
	{
		getversion_one_rhs();
	}
	else /* Rhs == 2 */
	{
		getversion_two_rhs();
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int getversion_no_rhs(void)
{
	static char Version[]=SCI_VERSION; 
	static int n1,m1;
	char *v = Version ;

	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Version), &m1),&n1,&v);
	LhsVar(1) = Rhs+1;
	if (Lhs==2) 
	{
		static char *Str[12];
		static int irep,nbuf;
		char tk[]="tk";
		char pvm[]="pvm";
		char modelicac[]="modelicac";
		char javasci[]="javasci";

		#ifdef WITH_ATLAS
		char atlas[]="atlas";
		#endif

		#ifdef NDEBUG
		char releasemode[]="release";
		#else
		char debugmode[]="debug";
		#endif 

		#ifdef __TIME__
			char TimeBuild[]=__TIME__;
		#endif

		#ifdef __DATE__
			char DateBuild[]=__DATE__;
		#endif

		
		n1=0;

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

		#ifdef __TIME__
		{
			Str[n1]=DateBuild;
			n1++;
		}
		#endif

		#ifdef __DATE__
		{
			Str[n1]=TimeBuild;
			n1++;
		}
		#endif

		m1=1;
		CreateVarFromPtr(Rhs+ 2,MATRIX_OF_STRING_DATATYPE, &m1, &n1, Str);
		LhsVar(2) = Rhs+2;
	}
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int getversion_one_rhs(void)
{
	if (Lhs == 2)
	{
		error_scilab(78,"%s : wrong number of lhs arguments","getversion");
		return 0;
	}

	if (GetType(1) == sci_strings)
	{
		static int l1,n1,m1;
		char *Param=NULL;
		int *VERSIONMATRIX=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		Param=cstk(l1);

		VERSIONMATRIX=(int *)MALLOC( (4)*sizeof(int) );

		if (strcmp(Param,"scilab") == 0)
		{
			VERSIONMATRIX[0]=(int)SCI_VERSION_MAJOR;
			VERSIONMATRIX[1]=(int)SCI_VERSION_MINOR;
			VERSIONMATRIX[2]=(int)SCI_VERSION_MAINTENANCE;
			VERSIONMATRIX[3]=(int)SCI_VERSION_REVISION; 
		}
		else if (with_module(Param))
		{
			char versionstring[1024];

			int version_module_major=0;
			int version_module_minor=0;
			int version_module_maintenance=0;
			int version_module_revision=0;

			if (getversionmodule(Param,&version_module_major,&version_module_minor,&version_module_maintenance,versionstring,&version_module_revision))
			{
				VERSIONMATRIX[0]=version_module_major;
				VERSIONMATRIX[1]=version_module_minor;
				VERSIONMATRIX[2]=version_module_maintenance;
				VERSIONMATRIX[3]=version_module_revision; 
			}
			else
			{
				error_scilab(999,"Error : file VERSION in %s.",Param);
				return 0;
			}
		}
		else
		{
			error_scilab(999,"Error : incorrect module name %s.",Param);
			return 0;
		}
		m1=1;
		n1=4;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1 ,&VERSIONMATRIX);
		LhsVar(1)=Rhs+1;
		PutLhsVar();
		if (VERSIONMATRIX){	FREE(VERSIONMATRIX); VERSIONMATRIX=NULL;}
	}
	else
	{
		error_scilab(999,"invalid argument.");
		return 0;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int getversion_two_rhs(void)
{
	if (Lhs == 2)
	{
		error_scilab(78,"%s : wrong number of lhs arguments","getversion");
		return 0;
	}

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		static int l2,n2,m2;
		char *ParamRhs2=NULL;

		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		ParamRhs2=cstk(l2);

		if (strcmp(ParamRhs2,VERSION_STRING)==0)
		{
			static int l1,n1,m1;
			char *ParamRhs1=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			ParamRhs1=cstk(l1);

			if (strcmp(ParamRhs1,"scilab") == 0)
			{
				char *output=NULL ;
				output=(char*)MALLOC((strlen(SCI_VERSION_STRING)+1)*sizeof(char));
				sprintf(output,"%s",SCI_VERSION_STRING);

				n1=1;
				CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
				if (output) {FREE(output);output=NULL;}

				LhsVar(1) = Rhs+1;
				C2F(putlhsvar)();
				return 0;
			}
			else if (with_module(ParamRhs1))
			{
				#define LineMax 1024
				char versionstring[LineMax];

				int version_module_major=0;
				int version_module_minor=0;
				int version_module_maintenance=0;
				int version_module_revision=0;

				if (getversionmodule(ParamRhs1,&version_module_major,&version_module_minor,&version_module_maintenance,versionstring,&version_module_revision))
				{
					char *output=NULL ;
					output=(char*)MALLOC((LineMax+1)*sizeof(char));
					sprintf(output,"%s",versionstring);

					n1=1;
					CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
					if (output) {FREE(output);output=NULL;}

					LhsVar(1) = Rhs+1;
					C2F(putlhsvar)();
					return 0;
				}
				else
				{
					error_scilab(999,"Error : file VERSION in %s.",ParamRhs1);
					return 0;
				}
			}
			else
			{
				error_scilab(999,"Error : incorrect module name %s.",ParamRhs1);
				return 0;
			}
		}
		else
		{
			error_scilab(999,"incorrect second parameter.");
			return 0;
		}
	}
	else
	{
		error_scilab(999,"invalid argument(s).");
		return 0;
	}
}
/*-----------------------------------------------------------------------------------*/
