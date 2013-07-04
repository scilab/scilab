// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 10178 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10178
//
// <-- Short Description -->
//    norm function falied for some sparse matrices.
// =============================================================================

A = [1 0 0 1 0;0 -1 -1 0 -1];
AS = sparse(A);

assert_checkalmostequal(norm(A), norm(AS));
assert_checkalmostequal(norm(A'), norm(AS'));
assert_checkequal(norm(A, 1), norm(AS, 1));
assert_checkequal(norm(A, %inf), norm(AS, %inf));
assert_checkequal(norm(A, 'fro'), norm(AS, 'fro'));

A = [1 1 1 1 1;-1 -1 -1 -1 -1];
AS = sparse(A);

assert_checkalmostequal(norm(A), norm(AS));
assert_checkalmostequal(norm(A)', norm(AS)');
assert_checkequal(norm(A, 1), norm(AS, 1));
assert_checkequal(norm(A, %inf), norm(AS, %inf));
assert_checkequal(norm(A, 'fro'), norm(AS, 'fro'));


A = [
    0.    0.    0.    0.    0.  
    0.    0.    0.    0.    0.  
    0.    0.   -1.    0.    1.  
    0.    0.    0.    0.    0.  
    0.    0.    1.    0.   -1.  ];

AS = sparse(A);

assert_checkequal(norm(A), norm(AS));
assert_checkequal(norm(A, 1), norm(AS, 1));
assert_checkequal(norm(A, %inf), norm(AS, %inf));
assert_checkequal(norm(A, 'fro'), norm(AS, 'fro'));
