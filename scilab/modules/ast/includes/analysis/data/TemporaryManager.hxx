/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
