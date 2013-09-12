/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifndef _MSC_VER
#include <stdint.h>
#endif
#include "mput.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "islittleendian.h"
#include "convert_tools.h"
#include "localization.h"
#include "warningmode.h"
/*--------------------------------------------------------------------------*/
extern int swap; /* defined in mget.c */
/*--------------------------------------------------------------------------*/

/*===============================================
 * function to write data without type conversion
 *===============================================*/
#define MPUT_CHAR_NC(Type)			\
  {						\
    Type *val = (Type *) res ;			\
    fwrite(val,sizeof(Type),n,fa);		\
  }
/*--------------------------------------------------------------------------*/
#define MPUT_NC(Type) {					\
    Type *val = (Type *) res ;				\
    if ( swap) {					\
      unsigned long long temp;				\
      for ( i=0; i< n; i++) {				\
	swap_generic((char *)val,(char *)&temp, sizeof(Type));	\
        val++;						\
	fwrite(&temp,sizeof(Type),1,fa);		\
      }							\
    }							\
    else fwrite(val,sizeof(Type),n,fa);			\
  }
/*--------------------------------------------------------------------------*/
#define MPUT_GEN_NC(Type,cf)						\
  switch ( cf )								\
    {									\
    case ' ':								\
      MPUT_NC(Type); break;						\
    case 'b':								\
      swap = (islittleendian()==1) ? 1 : 0;				\
    MPUT_NC(Type); break;						\
    case 'l':								\
      swap = (islittleendian()==1) ? 0 : 1;				\
    MPUT_NC(Type); break;						\
    default:								\
      sciprint(_("%s: Wrong value for input argument #%d (%s): '%s' or '%s' or '%s' expected.\n"),"mput",4,type," ","b","l"); \
      *ierr=1;return;							\
    }
/*--------------------------------------------------------------------------*/
/* write data without conversion (res is supposed to have type type) */
void C2F(mputnc) (int *fd, void * res, int *n1, char *type, int *ierr)
{
    char c1, c2;
    int i, n;
    FILE *fa;
    n = *n1;
    *ierr = 0;
    if ((fa = GetFileOpenedInScilab(*fd)) == NULL)
    {
        if ( getWarningMode() )
        {
            sciprint(_("%s: No input file associated to logical unit %d.\n"), "mput", *fd);
        }
        *ierr = 3;
        return;
    }

    c1 = ( strlen(type) > 1) ? type[1] : ' ';
    c2 = ( strlen(type) > 2) ? type[2] : ' ';
    switch ( type[0] )
    {
        case 'i' :
            MPUT_GEN_NC(int, c1);
            break;
        case 'l' :
            MPUT_GEN_NC(long long, c1);
            break;
        case 's' :
            MPUT_GEN_NC(short, c1);
            break;
        case 'c' :
            MPUT_CHAR_NC(char) ;
            break;
        case 'd' :
            MPUT_GEN_NC(double, c1);
            break;
        case 'f' :
            MPUT_GEN_NC(float, c1);
            break;
        case 'u' :
            switch ( c1 )
            {
                case 'i' :
                    MPUT_GEN_NC(unsigned int, c2);
                    break;
                case 'l' :
                    MPUT_GEN_NC(unsigned long long, c2);
                    break;
                case 's' :
                    MPUT_GEN_NC(unsigned short, c2);
                    break;
                case ' ' :
                    MPUT_GEN_NC(unsigned int, ' ');
                    break;
                case 'c' :
                    MPUT_CHAR_NC(unsigned char);
                    break;
                default :
                    *ierr = 1;
                    return ;
            }
            break;
        default :
            *ierr = 1;
            break;
    }
}
/*--------------------------------------------------------------------------*/
/*================================================
 * function to write data stored in double
 *================================================*/
/** used for char **/
#define MPUT_CHAR(Type) {			\
    for ( i=0; i< n; i++) {			\
      Type  val = (char) *res++;		\
      fwrite(&val,sizeof(Type),1,fa);		\
    }						\
  }
/*--------------------------------------------------------------------------*/
/** write in a machine independant way : i.e data
    is swaped if necessary to output little-endian
    data **/

#define MPUT(Type) {						\
    Type val;							\
    for ( i=0; i< n; i++) {					\
      val =(Type)res[i];					\
      if ( swap) {						\
	unsigned long long temp;				\
	swap_generic((char *)&val,(char *)&temp, sizeof(Type));	\
	fwrite(&temp,sizeof(Type),1,fa);			\
      }								\
      else fwrite(&val,sizeof(Type),1,fa);			\
    }								\
  }

/*--------------------------------------------------------------------------*/
/** The output mode is controlled by type[1] **/
#define MPUT_GEN(Type,cf)						\
  switch ( cf )								\
    {									\
    case ' ':								\
      MPUT(Type); break;						\
    case 'b':								\
      swap = (islittleendian()==1) ? 1 : 0;				\
    MPUT(Type); break;							\
    case 'l':								\
      swap = (islittleendian()==1) ? 0 : 1;				\
    MPUT(Type); break;							\
    default:								\
      if ( getWarningMode() ) sciprint(_("%s: Wrong value for input argument #%d (%s): '%s' or '%s' or '%s' expected.\n"),"mput",4,type," ","b","l"); \
      *ierr=1;return;							\
    }
/*--------------------------------------------------------------------------*/
void mput2 (FILE *fa, int swap2, double *res, int n, char *type, int *ierr)
{
    char c1, c2;
    int i;
    *ierr = 0;
    c1 = ( strlen(type) > 1) ? type[1] : ' ';
    c2 = ( strlen(type) > 2) ? type[2] : ' ';
    switch ( type[0] )
    {
        case 'i' :
            MPUT_GEN(int, c1);
            break;
        case 'l' :
            MPUT_GEN(long long, c1);
            break;
        case 's' :
            MPUT_GEN(short, c1);
            break;
        case 'c' :
            MPUT_CHAR(char) ;
            break;
        case 'd' :
            MPUT_GEN(double, c1);
            break;
        case 'f' :
            MPUT_GEN(float, c1);
            break;
        case 'u' :
            switch ( c1 )
            {
                case 'i' :
                    MPUT_GEN(unsigned int, c2);
                    break;
                case 'l' :
                    MPUT_GEN(unsigned long long, c2);
                    break;
                case 's' :
                    MPUT_GEN(unsigned short, c2);
                    break;
                case ' ' :
                    MPUT_GEN(unsigned int, ' ');
                    break;
                case 'c' :
                    MPUT_CHAR(unsigned char);
                    break;
                default :
                    *ierr = 1;
                    return ;
            }
            break;
        default :
            *ierr = 1;
            break;
    }
}
/*--------------------------------------------------------------------------*/
void C2F(mput) (int *fd, double *res, int *n, char *type, int *ierr)
{
    int nc = 0, swap2 = 0;
    FILE *fa = NULL;
    *ierr = 0;
    if ((nc = (int)strlen(type)) == 0)
    {
        if ( getWarningMode() )
        {
            sciprint(_("%s: Wrong size for input argument #%d ('%s'): Non-empty string expected.\n"), "mput", 4, type);
        }
        *ierr = 2;
        return;
    }

    if ( *fd == -1  &&  GetFileOpenedInScilab(*fd) == NULL )
    {
        sciprint(_("%s: No File opened in Scilab.\n") , "mput" ) ;
        *ierr = 3 ;
        return ;
    }

    if ((fa = GetFileOpenedInScilab(*fd)) != NULL)
    {
        swap2 = GetSwapStatus(*fd);
        mput2(fa, swap2, res, *n, type, ierr);
        if (*ierr > 0)
        {
            if ( getWarningMode() )
            {
                sciprint(_("%s: Wrong value for input argument #%d ('%s'): Format not recognized.\n"), "mput", 4, type);
            }
        }
    }
    else
    {
        if ( getWarningMode() )
        {
            sciprint(_("%s: Error while opening, reading or writing '%s'.\n"), "mput", GetFileNameOpenedInScilab(*fd));
        }
        *ierr = 3;
    }
}
/*--------------------------------------------------------------------------*/


