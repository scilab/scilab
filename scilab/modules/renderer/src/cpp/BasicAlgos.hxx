/*------------------------------------------------------------------------*/
/* file: BasicAlgos.hxx                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Several C++ functions that parform some basic operations        */
/*------------------------------------------------------------------------*/

#ifndef _BASIC_ALGOS_HXX_
#define _BASIC_ALGOS_HXX_

class BasicAlgos
{
public:

  /**
   * Create a new array of char *, with all its elements set to NULL;
   */
  static char ** createStringArray(int nbElement);

  /**
   * copy a char * array into an other
   */
  static void stringArrayCopy(char * dest[], char * src[], int nbElement);

  /**
   * delete an array of strings
   */
  static void destroyStringArray(char * src[], int nbElement) ;

};

#endif /* _BASIC_ALGOS_HXX_ */
