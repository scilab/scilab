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

#ifndef __FUNCMANAGER_HH__
#define __FUNCMANAGER_HH__

#include <map>
#include <iostream>
#include "scilabFunction.hxx"

#define __SIZE_MAP__ 1000000
using namespace std;

class FuncManager
{
public:
	FuncManager(void);
	~FuncManager(void);

	bool ReadModulefile();

	map <string, ScilabFunction*> m_mapFuncList;

};

#endif //__FUNCMANAGER_HH__