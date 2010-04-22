/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file location.hxx
 ** Define the Location class.
 */

#ifndef __LOCATION_HXX__
#define __LOCATION_HXX__

#include <iostream>
#include <string>
#include <sstream>

#ifdef _MSC_VER
	#if AST_EXPORTS
		#define EXTERN_AST __declspec (dllexport)
	#else
		#define EXTERN_AST __declspec (dllimport)
	#endif
#else
	#define EXTERN_AST 
#endif

/** \brief Abstract a Location. */
class EXTERN_AST Location
{
  /** \name Ctor & dtor.
   ** \{ */
public:
  /** \brief Construct a Location. */
  Location (void)
  {
    first_line = last_line = 1;
    first_column = last_column = 1;
  }
  /** \} */


  /** \name Line and Column related manipulators
   ** \{ */
public:
  /** \brief Reset initial location to final location. */
  inline void step (void)
  {
    first_line = last_line;
    first_column = last_column;
  }

  /** \brief Extend the current location to the COUNT next columns. */
  inline void columns (unsigned int count = 1)
  {
    last_column += count;
  }

  /** \brief Extend the current location to the COUNT next lines. */
  inline void lines (unsigned int count = 1)
  {
    last_line += count;
  }
  /** \} */
	
  std::string location_string_get() const
  {
      std::ostringstream os;
      os << " (" << first_line << "," << first_column << ")";
      return std::string(os.str());
  }
  
public:
  /** \brief Beginning of the located region. */
  int first_line;
  int first_column;
  /** \brief End of the located region. */
  int last_line;
  int last_column;
};

#endif //! __LOCATION_HXX__
