// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2012 - Scilab Enterprises - Sylvestre Ledru
// Copyright (C) 2016, 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Perform a check on the size of the input arguments
assert_checktrue(execstr('linspace(2,[2,2])','errcatch')<>0);

// Basic use
assert_checkequal(linspace(1,2,-1),[]);
assert_checkequal(linspace(1,2,0),[]);
assert_checkequal(linspace(1,2,1),2);
assert_checkequal(linspace(1,2,2),[1,2]);
assert_checkequal(linspace(0,100,101),0:100);
computed = linspace(0,1,11);
expected = [0. 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.];
assert_checkalmostequal(computed,expected);

// Basic use with 12 and 14 which also test scaling
computed = linspace(12,14,11);
expected = [12. 12.2 12.4 12.6 12.8 13. 13.2 13.4 13.6 13.8 14.];
assert_checkalmostequal(computed,expected);

// Column inputs
assert_checkequal(size(linspace([0;2],[2;5],5)),[2,5]);
assert_checkequal(size(linspace([0;2],[2;5],2)),[2,2]);
assert_checkequal(linspace([0;2],[2;5],2),[0,2;2,5]);
assert_checkalmostequal(linspace([-1;2],[2;6],6), [-1,-0.4,0.2,0.8,1.4,2; 2,2.8,3.6,4.4,5.2,6]);

// With complex bounds
assert_checkequal(linspace(0,3-3*%i,4), (0:3)*(1-%i));
assert_checkequal(linspace(3-3*%i,0,7), (3:-0.5:0)*(1-%i));
expected = complex([1;1]*(1:-0.25:0), [0:0.5:2 ; 2:0.5:4]);
assert_checkequal(linspace(1+%i*[0;2],[2;4]*%i,5),expected);

// With encoded integers
a = linspace(uint8(5), uint8(127), 10);
e = uint8([5  18  32  45  59  72  86  99  113  127]);
assert_checkequal(a, e);
a = linspace(uint8(127), uint8(5), 10);
e2 = uint8([127  114  100  87  73  60  46  33  19  5]);
assert_checkequal(a, e2);
a = linspace(uint8([5;127]), uint8([127;5]), 10);
assert_checkequal(a, [e ; e2]);
   // tests with int64 and uint64 in the dedicated linspace_uint64.tst
