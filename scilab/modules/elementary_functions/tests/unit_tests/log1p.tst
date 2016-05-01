//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - John GLIKSBERG
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//
// <-- CLI SHELL MODE -->
//

// this corresponds to log(0.7) and should therefore be authorized.
// it was not on some versions of Scilab
log1p(-0.3);

// check that log1p(x) is close to log(1+x) on a few defined values
assert_checkalmostequal( log1p(-0.4), log(-0.4 + 1), 1e-5);
assert_checkalmostequal( log1p(-0.0), log(-0.0 + 1), 1e-5);
assert_checkalmostequal( log1p(+1.2), log(+1.2 + 1), 1e-5);
assert_checkalmostequal( log1p(+5.6), log(+5.6 + 1), 1e-5);

// this corresponds to log(0) which is and should be undefined
assert_checkerror("log1p(-1)", "log1p: Wrong value for input argument #1 : Singularity of the function.");
