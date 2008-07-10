/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Several C++ functions that parform some basic operations
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "BasicAlgos.hxx"
#include <string.h>

/*------------------------------------------------------------------------------------------*/
char ** BasicAlgos::createStringArray(int nbElement)
{
  char ** res = new char *[nbElement];

  for (int i = 0; i < nbElement; i++)
  {
    res[i] = NULL;
  }
  return res;
}
/*------------------------------------------------------------------------------------------*/
void BasicAlgos::stringArrayCopy(char * dest[], char * src[], int nbElement)
{
  int i ;
  for ( i = 0 ; i < nbElement ; i++ )
  {
    int elemSize =  (int) strlen( src[i] ) + 1 ;
    if (dest[i] != NULL) {delete[] dest[i]; }

    dest[i] = new char[elemSize] ;

    if ( dest[i] == NULL )
    {
      destroyStringArray( dest, nbElement ) ;
      return ;
    }

    strcpy( dest[i], src[i] ) ;
  }
}
/*------------------------------------------------------------------------------------------*/
void BasicAlgos::destroyStringArray(char * src[], int nbElement)
{
  if (src == NULL) {return;}

  for (int i = 0; i < nbElement; i++)
  {
    if (src[i] != NULL) {delete[] src[i];}
  }
  delete[] src;
}
/*------------------------------------------------------------------------------------------*/

