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
// unit tests for isscalar function
// =============================================================================

// Constants
assert_checkequal(isscalar([1 2 3]), %f);
assert_checkequal(isscalar([1 2 3]'), %f);
assert_checkequal(isscalar([1 2; 1 2]), %f);
assert_checkequal(isscalar(1), %t);
assert_checkequal(isscalar([]), %f);

// Strings
assert_checkequal(isscalar(["s" "s"; "s" "s"]), %f);
assert_checkequal(isscalar("s"), %t);

// Polynomials
s=poly(0,"s");
assert_checkequal(isscalar([s s; s s]), %f);
assert_checkequal(isscalar(1+s), %t);

// Booleans
assert_checkequal(isscalar([%t %t; %t %t]), %f);
assert_checkequal(isscalar(%t), %t);

// Sparses
assert_checkequal(isscalar(sparse([1 2; 1 2])), %f);
assert_checkequal(isscalar(sparse(0)), %t);

// Structures
clear s;
assert_checkequal(isscalar(struct()), %f); // Empty structure
s(1,1).a = "test";
assert_checkequal(isscalar(s), %t); // Scalar structure
clear s;
s(3,1).a = %pi;
assert_checkequal(isscalar(s), %f); // Column structure
clear s;
s(1,3).a = %e;
assert_checkequal(isscalar(s), %f); // Row structure
clear s;
s(3,2).a = %z;
assert_checkequal(isscalar(s), %f); // 2D structure array (not square)
clear s;
s(2,2,2).a = %s;
assert_checkequal(isscalar(s), %f); // 3D structure array (cube)
clear s;
s(2,1,2).a = %s;
assert_checkequal(isscalar(s), %f); // 3D structure array with singleton (square)

// Cells
assert_checkequal(isscalar(cell()), %f); // Empty cell
a = cell(1);
a{1} = 1;
assert_checkequal(isscalar(a), %t); // Scalar case
clear a;
a = cell(1,3);
a{1} = 1:3;
a{2} = 1:3;
a{3} = 1:3;
assert_checkequal(isscalar(a), %f); // Row case
clear a;
a = cell(3,1);
a{1} = 1:3;
a{2} = 1:3;
a{3} = 1:3;
assert_checkequal(isscalar(a), %f); // Column case
clear a;
a = cell(3,2);
a{1,1} = 1:3;
a{1,2} = 1:3;
a{2,1} = 1:3;
a{2,2} = 1:3;
a{3,2} = 1:3;
a{3,1} = 1:3;
assert_checkequal(isscalar(a), %f); // Matrix case
clear a;
a = cell(2,2,2);
a{1,1,1} =1:3;
a{1,2,1} =1:3;
a{2,1,1} =1:3;
a{2,2,1} =1:3;
a{1,1,2} =1:3;
a{1,2,2} =1:3;
a{2,1,2} =1:3;
a{2,2,2} =1:3;
assert_checkequal(isscalar(a), %f); // Cubic case
a = cell(2,1,2);
a{1,1,1}=1:3;
a{2,1,1}=1:3;
a{1,1,2}=1:3;
a{2,1,2}=1:3;
assert_checkequal(isscalar(a), %f); // Hypermatrix with singleton (square)

// Lists
assert_checkequal(isscalar(list()), %f); // Empty list
l=list(1);
assert_checkequal(isscalar(l), %t); // Scalar case
clear l;
l=list(1,"test");
assert_checkequal(isscalar(l), %f); // Column case

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"isscalar", 1);
assert_checkerror("isscalar()", errmsg1);
errmsg2 = msprintf(_("Wrong number of input arguments."));
assert_checkerror("isscalar(1,2)", errmsg2);
errmsg3 = msprintf(_("Wrong number of output arguments.\n"));
assert_checkerror("[r,b]=isscalar([1 2 3]);", errmsg3);
