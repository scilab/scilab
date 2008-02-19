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
