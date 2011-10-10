/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/
/*  PURPOSE
 *     interface code for some interpolation / approximation
 *     routines
 */
/*--------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
#include "interpolation.h"
#include "stack-c.h"
#include "getfastcode.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int good_order(double x[], int n)
{
  /*  test if x[i-1] < x[i] */
  static int first = 1;
  int i;
  static double inf;

  if ( first ) { inf = 1.0 / (double) (first - first) ; first = 0; }

  if (fabs(x[0]) == inf  ||  x[n-1] == inf)
    return ( 0 );

  for ( i = 1 ; i < n ; i++ )
    if ( ! (x[i-1] < x[i]) )   /* form of this test for detecting nan ... */
      return ( 0 );

  return ( 1 );
}
/*--------------------------------------------------------------------------*/
/*
 *  Routines spéciales :
 *      récupérer une hypermatrice réelle
 *      récupérer une string scilab sans convertion
 *      comparer une string scilab et une C string classique
 *  (elles ne modifient pas les variables correspondantes
 *   ce qui est fondamental pour que cette interface soit
 *   passée par référence).
 */
/*--------------------------------------------------------------------------*/
int get_rhs_real_hmat(int num, RealHyperMat *H)
{
  int il, il1, il2, il3,/* it, */lw;

  lw = num + Top - Rhs;
  il = iadr(*Lstk( lw ));
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));

  if ( *istk(il) != 17 )
    goto err;
  else if ( *istk(il+1) != 3 )  /* a hm mlist must have 3 fields */
    goto err;

  /*  get the pointers for the 3 fields */
  il1 = sadr(il+6);
  il2 = il1 + *istk(il+3) - 1;
  il3 = il1 + *istk(il+4) - 1;
  il1 = iadr(il1); il2 = iadr(il2); il3 = iadr(il3);

  /*  test if the first field is a matrix string with 3 components
   *  and that the first is "hm" (ie 17 22  in scilab char code)
   */
  if ( (*istk(il1) != sci_strings)  |  ((*istk(il1+1))*(*istk(il1+2)) != 3)  )
    goto err;
  else if ( *istk(il1+5)-1 != 2 )  /* 1 str must have 2 chars */
    goto err;
  else if ( *istk(il1+8) != 17  || *istk(il1+9) != 22 )
    goto err;

  /*  get the 2d field */
  if ( (*istk(il2) != 8)  |  (*istk(il2+3) != 4) )
    goto err;
  H->dimsize = (*istk(il2+1))*(*istk(il2+2));
  H->dims = istk(il2+4);

  /*  get the 3d field */
  if ( !( *istk(il3) == 1 && *istk(il3+3)==0) )
    goto err;

  H->size = (*istk(il3+1))*(*istk(il3+2));
  H->R = stk(sadr(il3+4));

  /* needed for Jpc stuff (putlhsvar) */
  Nbvars = Max(Nbvars,num);
  C2F(intersci).ntypes[num-1] = '$';
  C2F(intersci).iwhere[num-1] = *Lstk(lw);
  C2F(intersci).lad[num-1] = 0;  /* a voir ? */
  return 1;

 err:
  Scierror(999,_("Wrong type for input argument #%d: Real hypermatrix expected.\n"), num);
  return 0;
}
/*--------------------------------------------------------------------------*/
int get_rhs_scalar_string(int num, int *length, int **tabchar)
{
  int il, lw;

  lw = num + Top - Rhs;
  il = iadr(*Lstk( lw ));
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));

  if ( ! ( *istk(il) == sci_strings  &&  (*istk(il+1))*(*istk(il+2)) == 1 ) )
    {
      /* we look for a scalar string */
      Scierror(999,_("Wrong type for input argument #%d: String expected.\n"), num);
      return 0;
    }
  *length = *istk(il+5)-1;
  *tabchar = istk(il+6);

  Nbvars = Max(Nbvars,num);
  C2F(intersci).ntypes[num-1] = '$';
  C2F(intersci).iwhere[num-1] = *Lstk(lw);
  C2F(intersci).lad[num-1] = 0;
  return 1;
}
/*--------------------------------------------------------------------------*/
static int equal_scistring_and_string(int length, int *scistr,  char *str)
{
  /* compare a scistring with a classic C string */
  int i = 0, res = 0;

  if ( (int)strlen(str) != length )
    return 0;

  res = 1; i = 0;
  while (res && i < length)
    {
      res = (scistr[i] == convertAsciiCodeToScilabCode(str[i]));
      i++;
    }
  return (res);
}
/*--------------------------------------------------------------------------*/
int get_type(TableType *Tab, int dim_table, int *scistr, int strlength)
{
  int i = 0, found = 0;
  while ( !found  &&  i < dim_table )
    {
      found =  equal_scistring_and_string(strlength, scistr, Tab[i].str_type);
      i++;
    }
  if ( found )
    return ( Tab[i-1].type );
  else
    return ( UNDEFINED );
}
/*--------------------------------------------------------------------------*/







