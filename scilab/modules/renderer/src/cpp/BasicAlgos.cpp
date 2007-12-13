/*------------------------------------------------------------------------*/
/* file: BasicAlgos.cpp                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Several C++ functions that parform some basic operations        */
/*------------------------------------------------------------------------*/

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
    delete[] src[i];
  }
  delete[] src;
}
/*------------------------------------------------------------------------------------------*/
