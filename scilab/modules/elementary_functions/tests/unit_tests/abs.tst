// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for abs() function (absolute value, magnitude)
// =============================================================================

assert_checkfalse(execstr("abs()", "errcatch") == 0);
assert_checkfalse(execstr("abs(1,2)", "errcatch") == 0);

assert_checkequal(abs(10)     ,10);
assert_checkequal(abs(-4.3)   ,4.3);
assert_checkequal(abs(6+8*%i) ,10);
assert_checkequal(abs(6-8*%i) ,10);

assert_checkequal(abs([ 7.6 , 8*%i ; 6+8*%i 6-8*%i ]), [7.6 , 8 ; 10 , 10 ]);
assert_checkequal(abs([ 7.6 8*%i 6+8*%i 6-8*%i ]), [7.6 8 10 10 ]);
assert_checkequal(abs([ 7.6 ; 8*%i ; 6+8*%i ; 6-8*%i ]), [7.6 ; 8 ; 10 ; 10 ]);


assert_checktrue(isnan(abs(%nan)));
assert_checktrue(isnan(abs(-%nan)));

assert_checkequal(abs(%inf), %inf);
assert_checkequal(abs(-%inf), %inf);
