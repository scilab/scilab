/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include <assert.h>
#include "symbol.hxx"
#include "scope.hxx"
#include "types.hxx"
#include "int.hxx"

using namespace types;
using symbol::Symbol;
using symbol::Scope;

static int scope_test(void)
{

    Symbol scilab("scilab");
    Symbol scicos("scicos");

    Scope env;
    Scope scicos_ns ("SCICOS");

    int *piVal0		= NULL;
    int *piJaune		= NULL;

    Int zero(1, 1, &piVal0);
    piVal0[0] = 0;

    Int jaune(1, 1, &piJaune);
    piJaune[0] = 51;

    env.put(scicos, zero);
    env.put(scilab, jaune);
    scicos_ns.put(scicos, jaune);
    scicos_ns.put(scilab, zero);

    assert(env.get(scilab) == &jaune);
    assert(env.get(scicos) == &zero);
    assert(scicos_ns.get(scilab) == &zero);
    assert(scicos_ns.get(scicos) == &jaune);

    std::cout << "----------------" << std::endl;
    std::cout << "Scope ENV Overview :" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << env;
    std::cout << "----------------" << std::endl;
    std::cout << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Scope SCICOS Overview :" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << scicos_ns;
    std::cout << "----------------" << std::endl;

    return 0;
}


int main(void)
{
    scope_test();
    return 0;
}
