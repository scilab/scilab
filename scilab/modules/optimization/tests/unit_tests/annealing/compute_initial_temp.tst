//<-- CLI SHELL MODE -->

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- JVM_NOT_MANDATORY -->

deff('y=test_func(x)','y=x^2');

x0 = 10;

T_init = compute_initial_temp(x0, test_func, 0.8, 1000, []);
assert_checktrue(T_init <> %nan);
assert_checktrue(abs(T_init) <> %inf);
