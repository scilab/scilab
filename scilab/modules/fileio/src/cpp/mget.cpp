/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "filemanager.hxx"

extern "C"
{
#ifndef _MSC_VER
#include <stdint.h>
#else
#define int32_t long
#define uint32_t unsigned long
#endif
#include "mget.h"
#include "sciprint.h"
#include "islittleendian.h"
#include "convert_tools.h"
#include "localization.h"
#include "charEncoding.h"
#include "sci_malloc.h"
}
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
void C2F(mgetnc)(int* fd, void* res, int* n1, const char* type, int* ierr)
{
    char c1;
    char c2;
    int i;
    int items   = *n1;
    int n       = *n1;
    FILE* fa; // used in MGET_GEN_NC => MGET_NC
    types::File* pFile = FileManager::getFile(*fd);

    *ierr = 0;

    if (pFile == NULL || (fa = pFile->getFiledesc()) == NULL)
    {
        sciprint(_("%s: No input file associated to logical unit %d.\n"), "mget", *fd);
        *ierr = 3;
        return;
    }

    c1 = (strlen(type) > 1) ? type[1] : ' ';
    c2 = (strlen(type) > 2) ? type[2] : ' ';

    int swap = pFile->getFileSwap(); // used in MGET_GEN_NC => MGET_NC

    switch (type[0])
    {
        case 'i' :
            MGET_GEN_NC(int, c1);
            break;
        case 'l' :
            MGET_GEN_NC(long long, c1);
            break;
        case 's' :
            MGET_GEN_NC(short, c1);
            break;
        case 'c' :
            MGET_CHAR_NC(char);
            break;
        case 'd' :
            MGET_GEN_NC(double, c1);
            break;
        case 'f' :
            MGET_GEN_NC(float, c1);
            break;
        case 'u' :
            switch (c1)
            {
                case 'i' :
                    MGET_GEN_NC(unsigned int, c2);
                    break;
                case 'l' :
                    MGET_GEN_NC(unsigned long long, c2);
                    break;
                case 's' :
                    MGET_GEN_NC(unsigned short, c2);
                    break;
                case ' ' :
                    MGET_GEN_NC(unsigned int, ' ');
                    break;
                case 'c' :
                    MGET_CHAR_NC(unsigned char);
                    break;
                default :
                    *ierr = 1;
                    return;
            }
            break;
        default :
            *ierr = 1;
            return;
    }

    if (items != n)
    {
        *ierr = -items - 1 ;
        // sciprint("Read %d out of\n",items,n);
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
void mget2(FILE *fa, int swap, double *res, int n, const char *type, int *ierr)
{
    char c1, c2;
    int i, items = n;
    *ierr = 0;
    c1 = ( strlen(type) > 1) ? type[1] : ' ';
    c2 = ( strlen(type) > 2) ? type[2] : ' ';
    switch ( type[0] )
    {
        case 'i' :
            MGET_GEN(int, c1);
            break;
        case 'l' :
            MGET_GEN(long long, c1);
            break;
        case 's' :
            MGET_GEN(short, c1);
            break;
        case 'c' :
            MGET_CHAR(char);
            break;
        case 'd' :
            MGET_GEN(double, c1);
            break;
        case 'f' :
            MGET_GEN(float, c1);
            break;
        case 'u' :
            switch ( c1 )
            {
                case 'i' :
                    MGET_GEN(unsigned int, c2);
                    break;
                case 'l' :
                    MGET_GEN(unsigned long long, c2);
                    break;
                case 's' :
                    MGET_GEN(unsigned short, c2);
                    break;
                case ' ' :
                    MGET_GEN(unsigned int, ' ');
                    break;
                case 'c' :
                    MGET_CHAR(unsigned char);
                    break;
                default :
                    *ierr = 1;
                    return;
                    break;
            }
            break;
        default :
            *ierr = 1;
            return ;
    }
    if ( items != n )
    {
        *ierr = -(items) - 1 ;
        /** sciprint("Read %d out of\n",items,n); **/
    }
    return;
}
/*--------------------------------------------------------------------------*/
void C2F(mget) (int *fd, double *res, int *n, const char *type, int *ierr)
{
    *ierr = 0;
    if (strlen(type) == 0)
    {
        sciprint(_("%s: Wrong size for input argument #%d: Non-empty string expected.\n"), "mget", 4, type);
        *ierr = 2;
        return;
    }

    types::File* pFile = FileManager::getFile(*fd);
    if (pFile && pFile->getFiledesc())
    {
        mget2(pFile->getFiledesc(), pFile->getFileSwap(), res, *n, type, ierr);
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
