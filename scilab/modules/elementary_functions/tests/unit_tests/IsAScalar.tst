// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->
//
// unit tests for IsAScalar function
// =============================================================================

// Constants
assert_checkequal(IsAScalar([1 2 3]), %f);
assert_checkequal(IsAScalar([1 2 3]'), %f);
assert_checkequal(IsAScalar([1 2; 1 2]), %f);
assert_checkequal(IsAScalar(1), %t);

// Strings
assert_checkequal(IsAScalar("s"), %f);

// Polynomials
s=poly(0,"s");
assert_checkequal(IsAScalar(1+s), %f);

// Booleans
assert_checkequal(IsAScalar(%t), %f);

// Sparses
assert_checkequal(IsAScalar(sparse(0)), %f);

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"IsAScalar", 1);
assert_checkerror("IsAScalar()", errmsg1);
errmsg2 = msprintf(_("Wrong number of input arguments."));
assert_checkerror("IsAScalar(1,2)", errmsg2);
errmsg3 = msprintf(_("Wrong number of output arguments.\n"));
assert_checkerror("[r,b]=IsAScalar([1 2 3]);", errmsg3);
