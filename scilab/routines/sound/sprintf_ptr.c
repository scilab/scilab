/***************************************************************
								
    ftn_printf: code extraced from RLab and hacked for Scilab 
              by Jean-Philippe Chancelier 1998 and S. Steer. 

    Copyright (C) 1995  Ian R. Searle
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *************************************************************** */
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif 
#include <stdio.h>
#include <ctype.h>  /* isdigit */
#include "malloc.h"

#include "../machine.h"

typedef int (*PRINTER) __PARAMS((FILE *, char *,...));


#define VPTR void * 
#define TOO_SMALL_BUF 1
#define FORMAT_NOT_ALLOWED 2
#define MEM_LACK 3
#define NOT_ENOUGH_DATA 4

/*---------- types and defs for doing printf ------------*/
#define  PF_C		0
#define  PF_S		1
#define  PF_D		2	/* int conversion */
#define  PF_LD		3	/* long int */
#define  PF_F		4	/* float conversion */

/* for switch on number of '*' and type */

#define  AST(num,type)  (5*(num)+(type))
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
The vsprintf_ptr is similar to standard sprintf procedure but the
argument other than string and format are passed by reference instead
of by value.

The type of data is determined by the format required for them.

The floating point values pointers are assumed to be pointers on double

This function may be called by a fortran routine.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifdef __STDC__ 
int C2F(sprintf_ptr)(char *buf,char *format,...) 
#else 
     /*VARARGS0*/
     int C2F(sprintf_ptr)(va_alist) va_dcl
#endif 
{
  va_list ap;
  int retval;
#ifdef __STDC__
  va_start(ap,format);
#else
  char *format;
  char *buf;
  va_start(ap);
  buf = va_arg(ap, char *);
  format = va_arg(ap, char *);
#endif
  retval=C2F(vsprintf_ptr)(buf,format,&ap);
  va_end(ap);
  return(retval);
}


#ifdef __STDC__ 
int C2F(vsprintf_ptr)(char * buf, char * format, va_list * ap)

#else 
     /*VARARGS0*/
     int C2F(vsprintf_ptr)(buf,format,ap)
     char *buf;
     char *format;
     va_list *ap;
#endif 
{
  int m1;
  int *m1p;
  char save;
  char *p, *fmt;
  register char *q;
  char *target;
  int l_flag, h_flag;		/* seen %ld or %hd  */
  int ast_cnt;
  int ast[2];
  double dval = 0.0;
  double * dvalp;
  int ival=0;
  int *ivalp;
  char *sval= "bug";
  int num_conversion = 0;	/* for error messages */
  int pf_type = 0;		/* conversion type */
  PRINTER printer;		

  int retval;


  if ( (fmt = (char *) malloc(strlen(format) + 1))== NULL) 
    return MEM_LACK;
  strcpy(fmt,format);

  q = fmt;
  retval = 0;

  target = buf;
  printer = (PRINTER) sprintf;
 
  /* Traverse format string, doing sprintf(). */
  while (1)
    {
      while (*q != '%')
	{
	  if (*q == 0)
	    {
	      /* really done */
	      *target = '\0';
	      goto END;
	    }
	  else
	    {
	      *target++ = *q++;
	      retval++;
	    }
	}

      num_conversion++;

      if (*++q == '%')		/* %% */
	{
	  *target++ = *q;
	  q++;
	  continue;
	}

      /* 
       * We have found a conversion specifier, figure it out,
       * then print the data asociated with it.
       */


      /* Get The data object from the arg-list */

      /* mark the '%' with p */
      p = q - 1;

      /* eat the flags */
      while (*q == '-' || *q == '+' || *q == ' ' ||
	     *q == '#' || *q == '0')
	q++;

      ast_cnt = 0;		/* asterisk count */
      if (*q == '*')
	{
	  /* Use current arg as field width spec */
	  if ((m1p = va_arg(*ap,int *)) == (int *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  ast[ast_cnt++] = *m1p;
	  q++;

	}
      else
	while ( isdigit(((int)*q)))  q++;
      /* width is done */

      if (*q == '.')		/* have precision */
	{
	  q++;
	  if (*q == '*')
	    {
	      /* Use current arg as precision spec */
	      if ((m1p = va_arg(*ap,int *)) == (int *) 0) {
		retval = NOT_ENOUGH_DATA;
		goto END;
	      }
	      ast[ast_cnt++] = *m1p;
	      q++;

	    }
	  else
	    while ( isdigit(((int)*q)) ) q++;
	}

      l_flag = h_flag = 0;

      if (*q == 'l')
	{
	  q++;
	  l_flag = 1;
	}
      else if (*q == 'h')
	{
	  q++;
	  h_flag = 1;
	}

      /* Set pf_type and load val */
      switch (*q++)
	{
	case 's':
	  if (l_flag + h_flag) {
	    /* bad conversion l or h flag mixed with s directive */
	    retval = FORMAT_NOT_ALLOWED;
	    goto END;
	  }
	  if ((sval = va_arg(*ap,char *)) == (char *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  pf_type = PF_S;
	  break;
	case 'c':
	  if (l_flag + h_flag){
	    /* bad conversion l or h flag mixed with c directive */
	    retval = FORMAT_NOT_ALLOWED;
	    goto END;
	  }
	  if ((sval = va_arg(*ap,char *)) == (char *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  pf_type = PF_C;
	  break;
	case 'd':
	  if ((ivalp = va_arg(*ap,int *)) == (int *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  ival=*ivalp;
	  pf_type = PF_D;
	  break;

	case 'o':
	  /* format not allowed */
	  retval = FORMAT_NOT_ALLOWED;
	  goto END;

	case 'x':
	  if ((ivalp = va_arg(*ap,int *)) == (int *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  ival=*ivalp;
	  pf_type = PF_D;
	  break;

	case 'X':
	  if ((ivalp = va_arg(*ap,int *)) == (int *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  ival=*ivalp;
	  pf_type = PF_D;
	  break;

	case 'i':
	case 'u':
	  /* use strod() here */
	  if ((ivalp = va_arg(*ap,int *)) == (int *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  ival=*ivalp;
	  pf_type = l_flag ? PF_LD : PF_D;
	  break;

	case 'e':
	case 'g':
	case 'f':
	case 'E':
	case 'G':
	  if (h_flag + l_flag)
	    {
	      /*  bad conversion */
	      retval =  FORMAT_NOT_ALLOWED;
	      goto END;
	    }
	  /* use strod() here */
	  if ((dvalp = va_arg(*ap,double *)) == (double *) 0) {
	    retval = NOT_ENOUGH_DATA;
	    goto END;
	  }
	  dval=*dvalp;
	  pf_type = PF_F;
	  break;

	default:
	  /*  bad conversion */
	  retval =  FORMAT_NOT_ALLOWED;
	  goto END;
	}

      save = *q;
      *q = 0;

      /* ready to call printf() */
      /* 
       * target:   The output file (or variable for sprintf())
       * p:        the beginning of the format
       * ast:      array with asterisk values
       */
      switch (AST (ast_cnt, pf_type))
	{
	case AST (0, PF_C):
	  retval += (*printer) ((VPTR) target, p, sval[0]);
	  break;

	case AST (1, PF_C):
	  retval += (*printer) ((VPTR) target, p, ast[0], sval[0]);
	  break;

	case AST (2, PF_C):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1],sval[0]);
	  break;

	case AST (0, PF_S):
	  retval += (*printer) ((VPTR) target, p, sval);
	  break;

	case AST (1, PF_S):
	  retval += (*printer) ((VPTR) target, p, ast[0], sval);
	  break;

	case AST (2, PF_S):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], sval);
	  break;

	case AST (0, PF_D):
	  retval += (*printer) ((VPTR) target, p, ival);
	  break;

	case AST (1, PF_D):
	  retval += (*printer) ((VPTR) target, p, ast[0],  ival);
	  break;

	case AST (2, PF_D):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], ival);
	  break;

	case AST (0, PF_LD):
	  retval += (*printer) ((VPTR) target, p, ival);
	  break;

	case AST (1, PF_LD):
	  retval += (*printer) ((VPTR) target, p, ast[0], ival);
	  break;

	case AST (2, PF_LD):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], ival);
	  break;

	case AST (0, PF_F):
	  retval += (*printer) ((VPTR) target, p, dval);
	  break;

	case AST (1, PF_F):
	  retval += (*printer) ((VPTR) target, p, ast[0], dval);
	  break;

	case AST (2, PF_F):
	  retval += (*printer) ((VPTR) target, p, ast[0], ast[1], dval);
	  break;
	}
	while (*target)
	  target++;
	*q = save;
    }
 END: 
  free(fmt);
  va_end(*ap);
  printf("%s\n",buf);
  return(retval);

}


#ifdef __STDC__ 
void C2F(fsciprint)(char *fmt,...)
#else 
     /*VARARGS0*/
     int C2F(fsciprint)(va_alist) va_dcl
#endif 
{
  va_list ap;
  int i;
  char s_buf[1024];
  int lstr;

#ifdef __STDC__
  va_start(ap,fmt);
#else
  char *fmt;
  va_start(ap);
  fmt = va_arg(ap, char *);
#endif

  C2F(vsprintf_ptr)(s_buf, fmt, &ap);

  i=0;/*C2F(xscion)(&i);*/
  if (i == 0) 
    {
      fprintf(stdout, "%s", s_buf);
    }
  else 
    {
      lstr=strlen(s_buf);
      /*C2F(xscisncr)(s_buf,&lstr,0L);*/
    }
  va_end(ap); 
}
