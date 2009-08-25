/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <ctype.h>  
#include <math.h>
#include "machine.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "Scierror.h"
#include "Scierror.h"
#include "scilabmode.h"
#include "localization.h"
#include "set_xxprintf.h"
#include "scistrtostr.h"
#include "charEncoding.h"

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define  PF_C		0
#define  PF_S		1
#define  PF_D		2
#define  PF_LD		3
#define  PF_F		4
/*--------------------------------------------------------------------------*/
#define  MAX_SPRINTF_SIZE  bsiz
char sprintf_buffer[MAX_SPRINTF_SIZE];
static char *sprintf_limit = sprintf_buffer + MAX_SPRINTF_SIZE;
/*--------------------------------------------------------------------------*/
static int GetScalarInt(char *fname,int *first,int *arg,int narg, int *ir,int ic,int *ival);
static int GetString (char *fname,int *first,int *arg,int narg, int *ir,int ic,char **sval);
static int GetScalarDouble(char *fname,int *prev,int *arg,int narg, int *ic,int ir,double *dval);
static void error_on_rval(XXPRINTF xxprintf,FLUSH flush,char *target);
static char* readNextUTFChar(char* utfstream,int* size);
static int call_printf(XXPRINTF xxprintf,char *target,char *p,char *sval,int *asterisk,int asterisk_count,int conversion_type,double dval );
#ifndef signbit
/* signbit does not exist with VS 2008 */
static int signbit(double x);
#endif
/*--------------------------------------------------------------------------*/
static void error_on_rval(XXPRINTF xxprintf,FLUSH flush,char *target)
{
	(*xxprintf) ((VPTR) target, "\n");
	(*flush) ((FILE *) target);
	Scierror(998,_("%s: Not enough arguments.\n"),"printf");
}
/*--------------------------------------------------------------------------*/
static int call_printf(XXPRINTF xxprintf,char *target,char *p,char *sval,int *asterisk,int asterisk_count,int conversion_type,double dval )
{
	/* for switch on number of '*' and type */
#define  choosetype(num,type)  (5*(num)+(type))

	int retval=-1;

	switch (choosetype (asterisk_count, conversion_type))
	{
	case choosetype (0, PF_S):
		retval += (*xxprintf) ((VPTR) target, p, sval);
		FREE(sval);
		break;

	case choosetype (1, PF_S):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], sval);
		FREE(sval);
		break;

	case choosetype (2, PF_S):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], asterisk[1], sval);
		FREE(sval);
		break;

	case choosetype (0, PF_C):
		retval += (*xxprintf) ((VPTR) target, p, sval[0]);
		FREE(sval);
		break;

	case choosetype (1, PF_C):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], sval[0]);
		FREE(sval);
		break;

	case choosetype (2, PF_C):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], asterisk[1],sval[0]);
		FREE(sval);
		break;

	case choosetype (0, PF_D):
		{
			retval += (*xxprintf) ((VPTR) target, p, (long long)dval);
		}
		break;

	case choosetype (1, PF_D):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], (int) dval);
		break;

	case choosetype (2, PF_D):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], asterisk[1], (int) dval);
		break;

	case choosetype (0, PF_LD):
		retval += (*xxprintf) ((VPTR) target, p, (long int) dval);
		break;

	case choosetype (1, PF_LD):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], (long int) dval);
		break;

	case choosetype (2, PF_LD):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], asterisk[1], (long int) dval);
		break;

	case choosetype (0, PF_F):
		retval += (*xxprintf) ((VPTR) target, p, dval);
		break;

	case choosetype (1, PF_F):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], dval);
		break;

	case choosetype (2, PF_F):
		retval += (*xxprintf) ((VPTR) target, p, asterisk[0], asterisk[1], dval);
		break;
	}
	return retval;
}
/*--------------------------------------------------------------------------*/
int do_xxprintf (char *fname, FILE *fp, char *format, int nargs, int argcount, int lcount, char **strv)
{
	int retval    = 0; /* return value */
	int arg_count = 0;
	int ccount    = 0;
	int prev      = 0;

	XXPRINTF xxprintf          = NULL; /* sprintf sciprint2 fprintf */
	FLUSH flush                = NULL;
	char *target               = NULL;
	register char *currentchar = NULL;

	currentchar = format;
	arg_count   = argcount;
	ccount      = 1;

	set_xxprintf(fp,&xxprintf,&flush,&target);

	/* "scan" string format. */
	while (TRUE)
	{
		char *p=NULL;
		char *sval=NULL;

		int ival      = 0;
		int low_flag  = 0;
		int	high_flag = 0;

		int asterisk_count = 0;
		int asterisk[2];
		int rval=0;

		int conversion_type           = 0;
		double dval                   = 0.0;
		register char *tmpcurrentchar = NULL;

		asterisk[0] = 0;
		asterisk[1] = 0;

		if (fp)
		{
			while (*currentchar != '%')
			{
				switch (*currentchar) 
				{
				case 0 : 
					fflush (fp);
					return (retval);
					break;
				case '\\':
					currentchar++;
					switch (*currentchar) 
					{
					case 0 : 
						fflush (fp);
						return (retval);
						break;
					case 'r':
						(*xxprintf) ((VPTR) target, "\r");
						currentchar++;
						retval++;
						break;
					case 'n':
#ifdef _MSC_VER
						if (getScilabMode() != SCILAB_STD)
						{
							if ( fp == stdout ) (*xxprintf) ((VPTR) target, "\r");
						}
#endif
						(*xxprintf) ((VPTR) target, "\n");
						currentchar++;
						retval++;
						break;
					case 't':
						(*xxprintf) ((VPTR) target, "\t");
						currentchar++;
						retval++;
						break;
					case '\\':
						(*xxprintf) ((VPTR) target, "\\");
						currentchar++;
						retval++;
						break;
					default:
						/* putc */
						(*xxprintf) ((VPTR) target, "%c",*currentchar);
						currentchar++;
						retval++;
					}
					break;

				default:
					/* putc */
					{
						int  charBytes = 0;
						char *UTFChar = NULL;
						char* outStr = NULL; /** locale char at most 2 bytes*/

						UTFChar = readNextUTFChar(currentchar,&charBytes);
						currentchar += charBytes;
						outStr = UTFChar;
						retval += charBytes;
						(*xxprintf) ((VPTR) target, "%s",outStr);
					}
					break;
				}
			}
		}
		else
		{
			/* sprintf() */
			while (*currentchar != '%') /* loop while we meet a % */
			{
				if (*currentchar == 0) /* End of the char * */
				{
					if (target > sprintf_limit)	/* over sprintf_limit */
					{
						Scierror(998,_("%s: An error occurred: %s\n"),fname,_("Buffer too small."));
						return RET_BUG;
					}
					else
					{
						/* done */
						*target = '\0';
						*strv = sprintf_buffer;
						return (retval);
					}
				}
				else
				{
					*target++ = *currentchar++;
					retval++;
				}
			}
		}

		if (*++currentchar == '%')	/* %% */
		{
			if (fp)
			{
				/* bug 2602 fixed */ 
				(*xxprintf) ((VPTR) target, "%c",*currentchar);
				retval++;
			}
			else
			{
				*target++ = *currentchar;
			}
			currentchar++;
			continue;
		}

		p = currentchar - 1;

		/* remove '-' '+' ' ' '#' '0' */
		while (*currentchar == '-' || *currentchar == '+' || *currentchar == ' ' || *currentchar == '#' || *currentchar == '0') currentchar++;

		asterisk_count = 0;	
		if (*currentchar == '*')
		{
			rval=GetScalarInt(fname,&prev,&arg_count,nargs,&ccount,lcount,&ival);

			if (rval <= 0) 
			{
				if (rval== NOT_ENOUGH_ARGS) 
				{
					error_on_rval(xxprintf,flush,target);
					return RET_BUG;
				}
				return rval;
			}

			asterisk[asterisk_count++] = ival;
			currentchar++;

		}
		else while ( isdigit(((int)*currentchar)))  currentchar++;

		if (*currentchar == '.')		/* precision */
		{
			currentchar++;
			if (*currentchar == '*')
			{
				rval=GetScalarInt(fname,&prev,&arg_count,nargs,&ccount,lcount,&ival);
				if (rval <= 0) {
					if (rval== NOT_ENOUGH_ARGS)
					{
						error_on_rval(xxprintf,flush,target);
						return RET_BUG;
					}
					return rval;
				}
				asterisk[asterisk_count++] = ival;
				currentchar++;
			}
			else while ( isdigit(((int)*currentchar)) ) currentchar++;
		}

		low_flag = high_flag = 0;

		if (*currentchar == 'l')
		{
			currentchar++;
			low_flag = 1;
		}
		else if (*currentchar == 'h')
		{
			currentchar++;
			high_flag = 1;
		}

		/* set conversion_type */
		tmpcurrentchar=currentchar;
		switch (*(currentchar++))
		{
		case 's': 
		case 'c':
			{
				if (low_flag + high_flag)
				{
					if (*tmpcurrentchar == 's')
					{
						Scierror(998,_("%s: Bad conversion 'l' or 'h' flag mixed with 's' directive.\n"),fname);
					}
					else /* 'c' */
					{
						Scierror(998,_("%s: Bad conversion 'l' or 'h' flag mixed with 'c' directive.\n"),fname);
					}
				}

				rval=GetString(fname,&prev,&arg_count,nargs,&ccount,lcount,&sval);

				if (rval <= 0) 
				{
					if (rval== NOT_ENOUGH_ARGS) 
					{
						error_on_rval(xxprintf,flush,target);
						return RET_BUG;
					}
					return rval;
				}

				if (*tmpcurrentchar == 's')
				{
					conversion_type = PF_S;
				}
				else /* 'c' */
				{
					conversion_type = PF_C;
				}
				break;
			}

		case 'd':
		case 'x':
		case 'X':
			rval = GetScalarDouble(fname, &prev, &arg_count, nargs, &ccount, lcount, &dval);
			if (rval <= 0) 
			{
				if (rval== NOT_ENOUGH_ARGS) 
				{
					error_on_rval(xxprintf, flush, target);
					return RET_BUG;
				}
				return rval;
			}
			conversion_type = PF_D;
			break;

		case 'i':
		case 'u':
			rval=GetScalarDouble(fname, &prev, &arg_count, nargs, &ccount, lcount, &dval);
			if (rval <= 0) 
			{
				if (rval== NOT_ENOUGH_ARGS)
				{
					error_on_rval(xxprintf, flush, target);
					return RET_BUG;
				}
				return rval;
			}
			conversion_type = low_flag ? PF_LD : PF_D;
			break;

		case 'e':
		case 'g':
		case 'f':
		case 'E':
		case 'G':
			if (high_flag + low_flag)
			{
				Scierror(998,_("%s: An error occurred: %s\n"),fname,_("Bad conversion."));
				return RET_BUG;
			}
			rval = GetScalarDouble(fname, &prev, &arg_count, nargs, &ccount, lcount, &dval);
			if (rval <= 0) 
			{
				if (rval== NOT_ENOUGH_ARGS)
				{
					error_on_rval(xxprintf,flush,target);
					return RET_BUG;
				}
				return rval;
			}
			conversion_type = PF_F;
			break;

		case 'o':
			Scierror(998,_("%s: An error occurred: %s\n"),fname,_("'o' format not allowed."));
			return RET_BUG;
			break;

		default:
			Scierror(998,_("%s: An error occurred: %s\n"),fname,_("Bad conversion."));
			return RET_BUG;
		}

		tmpcurrentchar=NULL;
		{
			char backupcurrentchar;
			backupcurrentchar = *currentchar;
			*currentchar = 0;

#define NanString "Nan"
#define InfString "Inf"
#define NegInfString "-Inf"
			/* print is not a string or a char */
			if ( (conversion_type != PF_S) && (conversion_type != PF_C) )
			{
				if (ISNAN(dval)) /* it is a %nan */
				{
					char formatnan[3] = "%s";
					char *valuenan = strdup(NanString);
					conversion_type = PF_S;
					dval = 0.;
					/* valuenan FREED in call_printf */
					call_printf(xxprintf,target,formatnan,valuenan,asterisk,asterisk_count,conversion_type,dval );
				}
				else
				{
					if (finite(dval) != 0) /* not %inf */
					{
						call_printf(xxprintf,target,p,sval,asterisk,asterisk_count,conversion_type,dval );
					}
					else /* %inf */
					{
						char formatinf[3] = "%s";
						char *valueinf = NULL;

						if ( signbit(dval) )
						{
							valueinf = strdup(NegInfString);
						}
						else
						{
							valueinf =  strdup(InfString);
						}

						conversion_type = PF_S;
						dval = 0.;
						/* valueinf FREED in call_printf */
						call_printf(xxprintf,target,formatinf,valueinf,asterisk,asterisk_count,conversion_type,dval );
					}
				} 
			}
			else
			{
				call_printf(xxprintf,target,p,sval,asterisk,asterisk_count,conversion_type,dval );
			}

			if (fp == (FILE *) 0) while (*target) target++;
			*currentchar = backupcurrentchar;
		}
	}
	return (retval);
}
/*--------------------------------------------------------------------------*/
static char* readNextUTFChar(char* utfstream,int* size)
{
	static char UTFChar[5]; /**UTF char. at most 4 bytes*/
	unsigned char charcode=(unsigned)*utfstream;
	/** UTF-8 format: ref. http://en.wikipedia.org/wiki/UTF-8/ */
	if(charcode > 193 && charcode <= 223 )
	{ /* twi bytes UTF-8 */
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]='\0';
		*size=2;
	}
	else if(charcode > 223 && charcode <= 239 )
	{/* three bytes UTF-8*/
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]=*(utfstream+2);;
		UTFChar[3]='\0';
		*size=3;
	}
	else if(charcode > 239 && charcode < 245 )
	{/* four bytes UTF-8*/
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]=*(utfstream+2);
		UTFChar[3]=*(utfstream+3);
		UTFChar[4]='\0';
		*size=4;
	}
	else
	{
		UTFChar[0]=*utfstream;
		UTFChar[1]='\0';
		*size=1;
	}
	return UTFChar;
}
/*--------------------------------------------------------------------------*/
static int GetScalarInt(char *fname, int *prev, int *arg, int narg, int *ic, int ir, int *ival)
{
	int mx,nx,lx;

	if (*prev != 1) 
	{
		*arg=*arg+1;*ic=1;
		*prev = 1;
	}

	GetRhsVar(*arg,MATRIX_OF_INTEGER_DATATYPE,&mx,&nx,&lx);

	if ( (*ic>nx) || (*prev != 1)) 
	{
		*arg=*arg+1;
		if (*arg > narg ) {
			return NOT_ENOUGH_ARGS;
		}
		*ic=1;
		GetRhsVar(*arg,MATRIX_OF_INTEGER_DATATYPE,&mx,&nx,&lx);
	}

	if (ir>mx) return RET_END;
	*ival =  *(istk(lx+ir-1+mx*(*ic-1)));
	*ic=*ic+1;
	return OK;
}
/*--------------------------------------------------------------------------*/
static int GetString(char *fname, int *prev, int *arg, int narg, int *ic, int ir, char **sval)
{
	int mx,nx,il,ild,lw,k,one=1;
	char *p;

	if (*prev != 2) 
	{
		*arg = *arg+1;*ic=1;
		*prev = 2;
	}
	lw = *arg + Top - Rhs;

	if (! C2F(getwsmat)(fname,&Top,&lw,&mx,&nx,&il,&ild,(unsigned long)strlen(fname))) {
		return RET_BUG;
	}
	else 
	{
		if ( *ic>nx ) 
		{
			*arg=*arg+1;
			if (*arg>narg ) {
				return NOT_ENOUGH_ARGS;
			}
			*ic=1;
			lw = *arg + Top - Rhs;
			if (! C2F(getwsmat)(fname,&Top,&lw,&mx,&nx,&il,&ild,(unsigned long) strlen(fname))) {
				return RET_BUG;
			}
		}
	}
	if (ir>mx) {
		return RET_END;
	}
	k=ir-1+mx*(*ic-1);
	if (SciStrtoStr(istk(il-1+*istk(ild+k)),&one,istk(ild+k),&p) < 0) {
		return MEM_LACK;
	}
	*ic=*ic+1;
	*sval = p;
	return OK;
}
/*--------------------------------------------------------------------------*/
static int GetScalarDouble(char *fname, int *prev, int *arg, int narg, int *ic, int ir, double *dval)
{
	int mx,nx,lx;

	if (*prev != 1) 
	{
		*arg = *arg+1;
		*ic=1;
		*prev = 1;
	}
	GetRhsVar(*arg,MATRIX_OF_DOUBLE_DATATYPE,&mx,&nx,&lx);
	if ( *ic>nx) 
	{
		*arg=*arg+1;
		if (*arg > narg ) {
			return NOT_ENOUGH_ARGS;
		}
		*ic=1;
		GetRhsVar(*arg,MATRIX_OF_DOUBLE_DATATYPE,&mx,&nx,&lx);
	}
	if (ir>mx) {
		return RET_END;
	}
	*dval =  *(stk(lx+ir-1+mx*(*ic-1)));
	*ic=*ic+1;
	return OK;
}
/*--------------------------------------------------------------------------*/
#ifndef signbit
static int signbit(double x)
{
	union
	{
		double d;
		short s[4];
		int i[2];
	} u;

	u.d = x;
#if SIZEOF_INT == 4
	return u.i[1] < 0;
#else
	return u.s[3] < 0;
#endif 
}
#endif /* signbit */
/*--------------------------------------------------------------------------*/
