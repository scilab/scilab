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
