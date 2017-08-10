// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function ret = test_macro(a, x)
    assert_checkequal(typeof(x), "function");
    ret = internal_macro(a) * 2;
endfunction

function ret = internal_macro(a)
    ret = a * 2;
endfunction

function ret = x()
    //nothing
endfunction