/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TEMPORARY_MANAGER_HXX__
#define __TEMPORARY_MANAGER_HXX__

#include <map>
#include <stack>
#include <unordered_map>

#include "TIType.hxx"
#include "TypeLocal.hxx"

#include "dynlib_ast.h"

namespace analysis
{

    class EXTERN_AST TemporaryManager
    {

	int currentId;
	std::map<TypeLocal, std::stack<int>> availableTmp;
	std::unordered_map<int, TypeLocal> usedTmp;

    public:

	TemporaryManager() : currentId(0) { }

	int getTmp(const TIType & type, const bool isAnInt = false);
	void releaseTmp(const int id);
	const std::map<TypeLocal, std::stack<int>> & getTemp() const;
    };

} // namespace analysis

#endif // __TEMPORARY_MANAGER_HXX__
