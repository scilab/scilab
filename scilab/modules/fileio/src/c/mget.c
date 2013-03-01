/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifndef _MSC_VER
#include <stdint.h>
#else
#define int32_t long
#define uint32_t unsigned long
#endif
#include "mget.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "islittleendian.h"
#include "convert_tools.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int swap = 0;

/*--------------------------------------------------------------------------*/
/* =================================================
* reads data and store them without type conversion
* =================================================*/
/*--------------------------------------------------------------------------*/
#define MGET_CHAR_NC(Type)				        \
  {								\
    Type *val = (Type *) res ;					\
    items=(int)fread(val,sizeof(Type),n,fa);			\
  }

/*--------------------------------------------------------------------------*/
#define MGET_NC(Type) {						\
    Type *val = (Type *) res ;					\
    if (swap) {							\
      items=0;							\
      for ( i=0; i< n; i++)  {					\
	unsigned long long tmp;					\
	items+=(int)fread(&tmp,sizeof(Type),1,fa);		\
	swap_generic((char *)&tmp,(char *)val, sizeof(Type));	\
	val++;							\
      }								\
    }								\
    else items=(int)fread(val,sizeof(Type),n,fa);		\
  }
/*--------------------------------------------------------------------------*/
#define MGET_GEN_NC(NumType,cf)			                \
  {								\
    switch (cf) {						\
    case ' ':							\
      MGET_NC(NumType);break;					\
    case 'b':							\
      swap = (islittleendian()==1)? 1:0;			\
    MGET_NC(NumType); break;					\
    case 'l':							\
      swap = (islittleendian()==1) ? 0:1;			\
    MGET_NC(NumType);  break;					\
    default:					          	\
      sciprint(_("%s: Wrong value for input argument #%d: '%s' or '%s' or '%s' expected.\n"),"mget",4," ","b","l"); \
      *ierr=1; return;						\
    }								\
}
/*--------------------------------------------------------------------------*/
void C2F(mgetnc) (int *fd, void *res, int *n1, char *type, int *ierr)
{
    char c1, c2;
    int i, items, n = *n1;
    FILE *fa;

    *ierr = 0;
    if ((fa = GetFileOpenedInScilab(*fd)) == NULL)
    {
        sciprint(_("%s: No input file associated to logical unit %d.\n"), "mget", *fd);
        *ierr = 3;
        return;
    }
    swap = GetSwapStatus(*fd);
    c1 = (strlen(type) > 1) ? type[1] : ' ';
    c2 = (strlen(type) > 2) ? type[2] : ' ';
    switch (type[0])
    {
        case 'i':
            MGET_GEN_NC(int, c1);

            break;
        case 'l':
            MGET_GEN_NC(int32_t, c1);
            break;
        case 's':
            MGET_GEN_NC(short, c1);

            break;
        case 'c':
            MGET_CHAR_NC(char);

            break;
        case 'd':
            MGET_GEN_NC(double, c1);

            break;
        case 'f':
            MGET_GEN_NC(float, c1);

            break;
        case 'u':
            switch (c1)
            {
                case 'i':
                    MGET_GEN_NC(unsigned int, c2);

                    break;
                case 'l':
                    MGET_GEN_NC(uint32_t, c2);
                    break;
                case 's':
                    MGET_GEN_NC(unsigned short, c2);

                    break;
                case ' ':
                    MGET_GEN_NC(unsigned int, ' ');

                    break;
                case 'c':
                    MGET_CHAR_NC(unsigned char);

                    break;
                default:
                    *ierr = 1;
                    return;
            }
            break;
        default:
            *ierr = 1;
            return;
    }
    if (items != n)
    {
        *ierr = -(items) - 1;
        /** sciprint("Read %d out of\n",items,n); **/
    }
    return;
}

/*--------------------------------------------------------------------------*/
/* =================================================
* reads data and store them in double
* =================================================*/

/* conversion macro  */
#define CONVGD(Type)				\
  {						\
    Type *val = (Type *) res ;			\
    for ( i = items-1 ; i >=0 ; i--)		\
      res[i] = val[i];				\
  }
/*--------------------------------------------------------------------------*/
#define MGET_GEN(NumType,cf)  MGET_GEN_NC(NumType,cf); CONVGD(NumType);
#define MGET_CHAR(NumType)    MGET_CHAR_NC(NumType);   CONVGD(NumType);
/*--------------------------------------------------------------------------*/
/* reads data and store them in double  */
void mget2(FILE * fa, int swap2, double *res, int n, char *type, int *ierr)
{
    char c1, c2;
    int i, items = n;

    *ierr = 0;
    c1 = (strlen(type) > 1) ? type[1] : ' ';
    c2 = (strlen(type) > 2) ? type[2] : ' ';
    switch (type[0])
    {
        case 'i':
            MGET_GEN(int, c1);

            break;
        case 'l':
            MGET_GEN(int32_t, c1);
            break;
        case 's':
            MGET_GEN(short, c1);

            break;
        case 'c':
            MGET_CHAR(char);

            break;
        case 'd':
            MGET_GEN(double, c1);

            break;
        case 'f':
            MGET_GEN(float, c1);

            break;
        case 'u':
            switch (c1)
            {
                case 'i':
                    MGET_GEN(unsigned int, c2);

                    break;
                case 'l':
                    MGET_GEN(uint32_t, c2);
                    break;
                case 's':
                    MGET_GEN(unsigned short, c2);

                    break;
                case ' ':
                    MGET_GEN(unsigned int, ' ');

                    break;
                case 'c':
                    MGET_CHAR(unsigned char);

                    break;
                default:
                    *ierr = 1;
                    return;
            }
            break;
        default:
            *ierr = 1;
            return;
    }
    if (items != n)
    {
        *ierr = -(items) - 1;
        /** sciprint("Read %d out of\n",items,n); **/
    }
    return;
}

/*--------------------------------------------------------------------------*/
void C2F(mget) (int *fd, double *res, int *n, char *type, int *ierr)
{
    int nc, swap2;
    FILE *fa;

    nc = (int)strlen(type);
    *ierr = 0;
    if (nc == 0)
    {
        sciprint(_("%s: Wrong size for input argument #%d: Non-empty string expected.\n"), "mput", 4, type);
        *ierr = 2;
        return;
    }
    fa = GetFileOpenedInScilab(*fd);
    if (fa)
    {
        swap2 = GetSwapStatus(*fd);
        mget2(fa, swap2, res, *n, type, ierr);
        if (*ierr > 0)
        {
            sciprint(_("%s: Wrong value for input argument #%d: Format not recognized.\n"), "mget", 4);
        }
    }
    else
    {
        sciprint(_("%s: No input file associated to logical unit %d.\n"), "mget", *fd);
        *ierr = 3;
    }
}

/*--------------------------------------------------------------------------*/
