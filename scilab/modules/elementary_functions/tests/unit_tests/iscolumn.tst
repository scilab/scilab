// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// unit tests for iscolumn function
// =============================================================================

// Constants
assert_checkequal(iscolumn([1; 2; 3]), %t);
assert_checkequal(iscolumn([1; 2; 3]'), %f);
assert_checkequal(iscolumn([1 2 3; 1 2 3]), %f);
assert_checkequal(iscolumn(1), %t);
assert_checkequal(iscolumn(rand(3,1,1)), %t);
assert_checkequal(iscolumn(rand(1,1,3)), %f);
assert_checkequal(iscolumn([]), %f);

// Strings
assert_checkequal(iscolumn(["s"; "t"; "u"]), %t);
assert_checkequal(iscolumn(["s"; "t"; "u"]'), %f);
assert_checkequal(iscolumn(["s" "t" "u";"s" "t" "u"]), %f);
assert_checkequal(iscolumn("s"), %t);
assert_checkequal(iscolumn(matrix(["s";"s";"s"], [3,1,1])), %t);
assert_checkequal(iscolumn(matrix(["s";"s";"s"], [1,1,3])), %f);

// Polynomials
s=poly(0,"s");
assert_checkequal(iscolumn([1+s; 2+s]), %t);
assert_checkequal(iscolumn([1+s; 2+s]'), %f);
assert_checkequal(iscolumn([1+s 2+s; 3+s 4+s]), %f);
assert_checkequal(iscolumn(1+s), %t);
assert_checkequal(iscolumn(matrix([s;s;s], [3,1,1])), %t);
assert_checkequal(iscolumn(matrix([s;s;s], [1,1,3])), %f);

// Booleans
assert_checkequal(iscolumn([%t; %f]), %t);
assert_checkequal(iscolumn([%t; %f]'), %f);
assert_checkequal(iscolumn([%t %f; %f %t]), %f);
assert_checkequal(iscolumn(%t), %t);
assert_checkequal(iscolumn(matrix([%t;%t;%t], [3,1,1])), %t);
assert_checkequal(iscolumn(matrix([%t;%t;%t], [1,1,3])), %f);

// Sparses
assert_checkequal(iscolumn(sparse([1; 0; 2])), %t);
assert_checkequal(iscolumn(sparse([1; 0; 2])'), %f);
assert_checkequal(iscolumn(sparse([1 0 2; 0 1 3])), %f);
assert_checkequal(iscolumn(sparse(0)), %t);

// Structures
clear s;
assert_checkequal(iscolumn(struct()), %f); // Empty structure
s.a = "test";
assert_checkequal(iscolumn(s), %t); // Scalar structure
clear s;
s(3,1).a = %pi;
assert_checkequal(iscolumn(s), %t); // Column structure
clear s;
s(3,1,1).a = %pi;
assert_checkequal(iscolumn(s), %t); // Column structure
clear s;
s(1,3).a = %e;
assert_checkequal(iscolumn(s), %f); // Row structure
clear s;
s(3,2).a = %z;
assert_checkequal(iscolumn(s), %f); // 2D structure array (not square)
clear s;
s(2,2,2).a = %s;
assert_checkequal(iscolumn(s), %f); // 3D structure array (cube)
clear s;
s(2,1,2).a = %s;
assert_checkequal(iscolumn(s), %f); // 3D structure array with singleton (square)

// Cells
assert_checkequal(iscolumn(cell()), %f); // Empty cell
a = cell(1);
a{1} = 1;
assert_checkequal(iscolumn(a), %t); // Scalar case
clear a;
a = cell(1,3);
a{1} = 1:3;
a{2} = 1:3;
a{3} = 1:3;
assert_checkequal(iscolumn(a), %f); // Row case
clear a;
a = cell(3,1);
a{1} = 1:3;
a{2} = 1:3;
a{3} = 1:3;
assert_checkequal(iscolumn(a), %t); // Column case
clear a;
a = cell(3,2);
a{1,1} = 1:3;
a{1,2} = 1:3;
a{2,1} = 1:3;
a{2,2} = 1:3;
a{3,2} = 1:3;
a{3,1} = 1:3;
assert_checkequal(iscolumn(a), %f); // Matrix case
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
assert_checkequal(iscolumn(a), %f); // Cubic case
a = cell(2,1,2);
a{1,1,1}=1:3;
a{2,1,1}=1:3;
a{1,1,2}=1:3;
a{2,1,2}=1:3;
assert_checkequal(iscolumn(a), %f); // Hypermatrix with singleton (square)

// Lists
assert_checkequal(iscolumn(list()), %f); // Empty list
l=list(1);
assert_checkequal(iscolumn(l), %t); // Scalar case
clear l;
l=list(1,"test");
assert_checkequal(iscolumn(l), %t); // Column case

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"iscolumn", 1);
assert_checkerror("iscolumn()", errmsg1);
errmsg2 = msprintf(_("Wrong number of input arguments."));
assert_checkerror("iscolumn(1,2)", errmsg2);
errmsg3 = msprintf(_("Wrong number of output arguments.\n"));
assert_checkerror("[r,b]=iscolumn([1 2 3]);", errmsg3);
