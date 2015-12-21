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

#ifndef __FBLOCK_EMITTED_LISTENER_HXX__
#define __FBLOCK_EMITTED_LISTENER_HXX__

#include "data/FunctionBlock.hxx"

namespace analysis
{

class EXTERN_AST FBlockEmittedListener
{

public:

    virtual void action(FunctionBlock & fblock) = 0;
};

} // namespace analysis

#endif // __FBLOCK_EMITTED_LISTENER_HXX__
