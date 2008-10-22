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

#include <list>
#include <vector>

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

  /**
   * Find an object inside a list.
   * @param searchedList, list in which the value is searched
   * @param value searched value
   * @return ture if the value is find, false otherwise
   */
  template <typename T>
  static bool listContains(std::list<T>& searchedList, T& value)
  {
    typename std::list<T>::iterator it = searchedList.begin();
    for (; it != searchedList.end(); it++)
    {
      if (*it == value)
      {
        return true;
      }
    }
    return false;
  }

  /**
   * Remove an object inside a vector.
   * @param searchedList, vector in which the value is searched
   * @param value searched value
   * @return ture if the value has been find and removed, false otherwise
   */
  template <typename T>
  static bool vectorRemove(std::vector<T>& searchedVector, T& value)
  {
    typename std::vector<T>::iterator it = searchedVector.begin();
    for (; it != searchedVector.end(); it++)
    {
      if (*it == value)
      {
        searchedVector.erase(it);
        return true;
      }
    }
    return false;
  }

};

#endif /* _BASIC_ALGOS_HXX_ */
