//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Checks performed with Octave v3.0.3

// With x as a row vector and 1 argument
x = [0.9    0.7  
    0.1    0.1  
    0.5    0.4];
computed = mtlb_var(x);
expected = [0.16 0.09];
assert_checkalmostequal(computed, expected, 1e-15);

// Normalization with N-1
x = [0.9    0.7  
    0.1    0.1  
    0.5    0.4];
w = 0;
computed = mtlb_var(x,w);
expected = [0.16 0.09];
assert_checkalmostequal(computed, expected, 1e-15);

// Normalization with N
x = [0.9    0.7  
    0.1    0.1  
    0.5    0.4];
w = 1;
computed = mtlb_var(x,w);
expected = [0.32 0.18] / 3;
assert_checkalmostequal(computed, expected, 1e-15);

// With x as a complex row vector and 1 argument
x = [ 0.9, 0.7;
0.1, 0.1;
0.5, 0.4 ];
x = x + x * 2 * %i;
computed = mtlb_var(x);
expected = [0.8 0.45];
assert_checkalmostequal(computed, expected, 1e-15);

// With x as a complex row vector and variance along the dimension 2, i.e. row by row and w= 0
x = [ 0.9, 0.7;
0.1, 0.1;
0.5, 0.4 ];
x = x + x * 2 * %i;
w = 0;
dim = 2;
computed = mtlb_var(x,w,dim);
expected = [0.1
0.
0.025];
assert_checkalmostequal(computed, expected, 1e-15);

// With x as a complex row vector and variance along the dimension 2, i.e. row by row and w= 1
x = [ 0.9, 0.7;
0.1, 0.1;
0.5, 0.4 ];
x = x + x * 2 * %i;
w = 1;
dim = 2;
computed = mtlb_var(x,w,dim);
expected = [0.05
0.
0.0125];
assert_checkalmostequal(computed, expected, 1e-15);

// With x as a row vector and 1 argument
x = [10 20 30 40 50 60 70 80 90];
computed = mtlb_var(x,0,2);
expected = 750;
assert_checkequal(computed, expected);

// With x as a column vector and 1 argument
x = [10; 20; 30; 40; 50; 60; 70; 80; 90];
computed = mtlb_var(x);
expected = 750;
assert_checkequal(computed, expected);
