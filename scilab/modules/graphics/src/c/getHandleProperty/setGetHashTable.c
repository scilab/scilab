/*------------------------------------------------------------------------*/
/* file: setGetHashTable.h                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : define two hash table to be used in sci_set and sci_get         */
/*        These hash table are based on the Scilab hashTable              */
/*------------------------------------------------------------------------*/

#include "setGetHashTable.h"
#include <string.h>

/*-----------------------------------------------------------------------------------*/
/* see http://www.cse.yorku.ca/~oz/hash.html */
/* like in hashtable_localization by Allan Cornet */
/* I choose djb2 algo. for strings */
static unsigned int setGetHashTableHash( void * key )
{
  unsigned long hash = 5381;
  int c ;
  char * str = key ;

  while ( c = *str++ )
  {
    hash = ((hash << 5) + hash) + c ; /* hash * 33 + c */
  }

  return hash;

}
/*-----------------------------------------------------------------------------------*/
static int setGetHashTableEqualKeys( void * k1, void * k2 )
{
  if ( strcmp( (char *)k1, (char *)k2 ) == 0 )
  {
    return 1 ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
GetPropertyHashTable * createGetHashTable( void )
{
  return create_hashtable(16, setGetHashTableHash, setGetHashTableEqualKeys ) ;
}
/*-----------------------------------------------------------------------------------*/
void destroyGetHashTable( GetPropertyHashTable * hashTable )
{
  /* we just store pointers */
  hashtable_destroy( hashTable, 0 ) ;
}
/*-----------------------------------------------------------------------------------*/
getPropertyFunc searchGetHashtable( GetPropertyHashTable * hashTable, char * key )
{
  return (getPropertyFunc) hashtable_search( hashTable, key ) ;
}
/*-----------------------------------------------------------------------------------*/
int insertGetHashtable( GetPropertyHashTable * hashTable, char * key, getPropertyFunc value )
{
  return hashtable_insert( hashTable, key, value ) ;
}
/*-----------------------------------------------------------------------------------*/
