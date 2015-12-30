/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
    types::String s42("42");
    std::cout << "s42 = " << d42.get(0, 0) << std::endl;
    assert(strcmp(s42.get(0, 0), "42") == 0);
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
