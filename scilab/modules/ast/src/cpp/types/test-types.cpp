/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

#include <iostream>
#include <assert.h>
#include "alltypes.hxx"

int main(void)
{

    /*
    ** types::Int
    */
    types::Int32 i42(1, 1);
    i42.set(0, 0, 42);
    std::cout << "i42 = " << ((long long*) i42.get())[0] << std::endl;
    assert(i42.get()[0] == 42);
    assert(i42.isInt() == true);
    assert(i42.isDouble() == false);

    /*
    ** types::Double
    */
    types::Double d42(42);
    std::cout << "d42 = " << d42.get(0, 0) << std::endl;
    assert(d42.get(0, 0) == 42);
    assert(d42.isInt() == false);
    assert(d42.isDouble() == true);

    /*
    ** types::String
    */
    types::String s42(L"42");
    std::cout << "s42 = " << d42.get(0, 0) << std::endl;
    assert(wcscmp(s42.get(0, 0), L"42") == 0);
    assert(s42.isInt() == false);
    assert(s42.isDouble() == false);
    assert(s42.isString() == true);

    /*
    ** types::Generic
    */
    /* -*- WARNING : GenericType() is protected -*- */
    //types::GenericType *gt = new types::GenericType();

    return 0;

}
