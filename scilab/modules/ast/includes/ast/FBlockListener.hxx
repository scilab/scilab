/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2017 - ESI - Antoine ELIAS
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

#ifndef __FBLOCK_LISTENER_HXX__
#define __FBLOCK_LISTENER_HXX__

#include "dynlib_ast.h"
#include "FBlockEmittedListener.hxx"

class EXTERN_AST FBlockListener : public analysis::FBlockEmittedListener
{

    static std::unordered_map<uint64_t, ast::Exp*> functions;
public:
    FBlockListener();
    virtual void action(analysis::FunctionBlock & fblock);
    static ast::Exp* getExp(uint64_t functionId);
};

#endif // __FBLOCK_LISTENER_HXX__


