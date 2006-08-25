/*------------------------------------------------------------------------*/
/* file: pixel_mode.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : make the correspondence between pixel mode name in scilab       */
/*        and the corresponding index                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "pixel_mode.h"

/**
* number of different pixel drawing modes.
*/
#define NB_PIXEL_MODES 16

/**
 * the array of correspondance
 */
static char * pmodes[NB_PIXEL_MODES] = { "clear"       , /* 1 */
                                         "and"         , /* 2 */
                                         "andReverse"  , /* 3 */
                                         "copy"        , /* 4 */
                                         "andInverted" , /* 5 */
                                         "noop"        , /* 6 */
                                         "xor"         , /* 7 */
                                         "or"          , /* 8 */
                                         "nor"         , /* 9 */
                                         "equiv"       , /* 10 */
                                         "invert"      , /* 11 */
                                         "orReverse"   , /* 12 */
                                         "copyInverted", /* 13 */
                                         "orInverted"  , /* 14 */
                                         "nand"        , /* 15 */
                                         "set"         , /* 16 */
                                       } ;

/*----------------------------------------------------------------------------------*/
char * getPixelMode( int index )
{
    return pmodes[index] ;
}
/*----------------------------------------------------------------------------------*/
int getPixelModeIndex( const char * modeName )
{
  /* I'm lazy. I won't use a hash table. Sorry if you are adding a new mode. */
  /* But I don't think it will happen often */

  int nameLen = 0 ;

  nameLen = strlen( modeName ) ;

  switch( modeName[0] )
  {
  case 'a' :
    {
      /* check intput is longer than the shorter string */
      if ( nameLen < 3 ) { break ; }
      switch( modeName[3] )
      {
      case 0 : /* and */
        return 2 ;
        break ;
      case 'R' : /* andReverse */
        return 3 ;
        break ;
      case 'I' : /* andInverted */
        return 5 ;
        break ;
      }
      break;
    }
  case 'c':
    {
      if ( nameLen < 4 ) { break ; }
      switch( modeName[4] )
      {
      case 'r' : /* clear */
        return 1 ;
        break ;
      case 0 : /* copy */
        return 4 ;
        break ;
      case 'I' : /* copyInverted */
        return 13 ;
        break ;
      }
      break ;
    }
  case 'e':
    {
      if ( nameLen < 5 ) { break ; }
      return 10 ; /* equiv */
      break ;
    }
  case 'i':
    {
      if ( nameLen < 6 ) { break ;}
      return 11 ; /* invert */
      break ;
    }
  case 'n':
    {
      if ( nameLen < 3 ) { break ; }
      switch( modeName[2] )
      {
      case 'o' : /* noop */
        return 6 ;
        break ;
      case 'r' : /* nor */
        return 9 ;
        break ;
      case 'n' : /* nand */
        return 15 ;
        break ;
      }
      break ;
    }
  case 'o':
    {
      if ( nameLen < 2 ) { break ; }
      switch( modeName[2] )
      {
      case 0 : /* or */
        return 8 ;
        break ;
      case 'R' : /* orReverse */
        return 12 ;
        break ;
      case 'I' : /* orInverted */
        return 14 ;
        break ;
      }
      break;
    }
  case 's':
    {
      if ( nameLen < 3 ) { break ; }
      return 16 ; /* set */
      break ;
    }
  case 'x':
    {
      if ( nameLen < 3 ) { break ; }
      return 7 ; /* xor */
      break ;
    }
  }
  return -1 ;
}
/*----------------------------------------------------------------------------------*/
int getNbPixelModes( void )
{
  return NB_PIXEL_MODES ;
}
/*----------------------------------------------------------------------------------*/
