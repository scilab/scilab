// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// unit tests for issquare function
// =============================================================================

// Constants
assert_checkequal(issquare([1 2 3]), %f);
assert_checkequal(issquare([1 2 3]'), %f);
assert_checkequal(issquare([1 2; 1 2]), %t);
assert_checkequal(issquare(1), %t);
assert_checkequal(issquare(rand(2,1,2)), %t);
assert_checkequal(issquare(rand(2,2,3)), %f);
assert_checkequal(issquare([]), %f);

// Strings
assert_checkequal(issquare(["s" "t" "u"]), %f);
assert_checkequal(issquare(["s" "t" "u"]'), %f);
assert_checkequal(issquare(["s" "t";"s" "t"]), %t);
assert_checkequal(issquare("s"), %t);
assert_checkequal(issquare(matrix(["s";"s";"s";"s"], [2,1,2])), %t);
assert_checkequal(issquare(matrix(["t";"t";"t";"t";"t";"t";"t";"t";"t";"t";"t";"t"], [2,2,3])), %f);

// Polynomials
s=poly(0,"s");
assert_checkequal(issquare([1+s 2+s]), %f);
assert_checkequal(issquare([1+s 2+s]'), %f);
assert_checkequal(issquare([1+s 2+s; 3+s 4+s]), %t);
assert_checkequal(issquare(1+s), %t);
assert_checkequal(issquare(matrix([s;s;s;s], [2,1,2])), %t);
assert_checkequal(issquare(matrix([s;s;s;s;s;s;s;s;s;s;s;s], [2,2,3])), %f);

// Booleans
assert_checkequal(issquare([%t %f]), %f);
assert_checkequal(issquare([%t %f]'), %f);
assert_checkequal(issquare([%t %f; %f %t]), %t);
assert_checkequal(issquare(%t), %t);
assert_checkequal(issquare(matrix([%t;%t;%t;%t], [2,1,2])), %t);
assert_checkequal(issquare(matrix([%t;%t;%t;%t;%t;%t;%t;%t;%t;%t;%t;%t], [2,2,3])), %f)

// Sparses
assert_checkequal(issquare(sparse([1 0 2])), %f);
assert_checkequal(issquare(sparse([1 0 2])'), %f);
assert_checkequal(issquare(sparse([1 0; 0 1])), %t);
assert_checkequal(issquare(sparse(0)), %t);

// Structures
clear s;
assert_checkequal(issquare(struct()), %f); // Empty structure
s.a = "test";
assert_checkequal(issquare(s), %t); // Scalar structure
clear s;
s(3,1).a = %pi;
assert_checkequal(issquare(s), %f); // Column structure
clear s;
s(1,3).a = %pi;
assert_checkequal(issquare(s), %f); // Row structure
clear s;
s(3,2).a = %z;
assert_checkequal(issquare(s), %f); // 2D structure array (not square)
clear s;
s(2,2,2).a = %s;
assert_checkequal(issquare(s), %t); // 3D structure array (cube)
clear s;
s(2,1,2).a = %s;
assert_checkequal(issquare(s), %t); // 3D structure array with singleton (square)

// Cells
assert_checkequal(issquare(cell()), %f); // Empty cell
a = cell(1);
a{1} = 1;
assert_checkequal(issquare(a), %t); // Scalar case
clear a;
a = cell(1,3);
a{1} = 1:3;
a{2} = 1:3;
a{3} = 1:3;
assert_checkequal(issquare(a), %f); // Row case
clear a;
a = cell(3,1);
a{1} = 1:3;
a{2} = 1:3;
a{3} = 1:3;
assert_checkequal(issquare(a), %f); // Column case
clear a;
a = cell(3,2);
a{1,1} = 1:3;
a{1,2} = 1:3;
a{2,1} = 1:3;
a{2,2} = 1:3;
a{3,2} = 1:3;
a{3,1} = 1:3;
assert_checkequal(issquare(a), %f); // Matrix case
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
assert_checkequal(issquare(a), %t); // Cubic case
a = cell(2,1,2);
a{1,1,1}=1:3;
a{2,1,1}=1:3;
a{1,1,2}=1:3;
a{2,1,2}=1:3;
assert_checkequal(issquare(a), %t); // Hypermatrix with singleton (square)

// Lists
assert_checkequal(issquare(list()), %f); // Empty list
l=list(1);
assert_checkequal(issquare(l), %t); // Scalar case
clear l;
l=list(1,"test");
assert_checkequal(issquare(l), %f); // Column case

// Error messages
errmsg1 = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"issquare", 1);
assert_checkerror("issquare()", errmsg1);
assert_checkerror("issquare(1,2)", errmsg1);
errmsg2 = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "issquare", 1);
assert_checkerror("[r,b]=issquare([1 2 3]);", errmsg2);
