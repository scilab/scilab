/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CONFIGVARIABLE_HXX__
#define __CONFIGVARIABLE_HXX__

#include <map>
#include <string>

#ifdef _MSC_VER
	#if SYSTEM_ENV_EXPORTS
		#define EXTERN_SYSTEM_ENV __declspec (dllexport)
	#else
		#define EXTERN_SYSTEM_ENV __declspec (dllimport)
	#endif
#else
	#define EXTERN_SYSTEM_ENV 
#endif

class EXTERN_SYSTEM_ENV ConfigVariable
{
private :
  ConfigVariable(){};
  ~ConfigVariable(){};

  static ConfigVariable *me;

  std::map<std::string, std::string>m_VarList;
public :

  static ConfigVariable* getInstance();

  std::string	get(const std::string& _szVar);
  bool		set(std::string _szVar, std::string _szVal);

};

#endif /* __CONFIGVARIABLE_HXX__ */
