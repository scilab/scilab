// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->


// 1. Test with a scalar function
function y = myfunction (x)
  y = x*x;
endfunction

x = 1.0;
expected = 2.0;
// 1.1 With default parameters
computed = derivative(myfunction,x);
assert_checkalmostequal ( computed , expected , 1.e-11 );
// 1.2 Test order 1
computed = derivative(myfunction,x,order=1);
assert_checkalmostequal ( computed , expected , 1.e-8 );
// 1.3 Test order 2
computed = derivative(myfunction,x,order=2);
assert_checkalmostequal ( computed , expected , 1.e-11 );
// 1.4 Test order 4
computed = derivative(myfunction,x,order=4);
assert_checkalmostequal ( computed , expected , %eps );

// 1.5 Compute second derivative at the same time
Jexpected = 2.0;
Hexpected = 2.0;
[Jcomputed , Hcomputed] = derivative(myfunction,x);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-11 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 1.6 Test order 1
[Jcomputed , Hcomputed] = derivative(myfunction,x,order=1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-8 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-6 );
// 1.7 Test order 2
[Jcomputed , Hcomputed] = derivative(myfunction,x,order=2);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-11 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 1.8 Test order 4
[Jcomputed , Hcomputed] = derivative(myfunction,x,order=4);
assert_checkalmostequal ( Jcomputed , Jexpected , %eps );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-11 );

// 2. Test with a vector function
function y = myfunction2 (x)
  y = x(1)*x(1) + x(2)+ x(1)*x(2);
endfunction
x = [1.0 
2.0];
expected = [4.0 2.0];
// 2.1 With default parameters
computed = derivative(myfunction2,x);
assert_checkalmostequal ( computed , expected , 1.e-10 );
// 2.2 Test order 1
computed = derivative(myfunction2,x,order=1);
assert_checkalmostequal ( computed , expected , 1.e-8 );
// 2.3 Test order 2
computed = derivative(myfunction2,x,order=2);
assert_checkalmostequal ( computed , expected , 1.e-10 );
// 2.4 Test order 4
computed = derivative(myfunction2,x,order=4);
assert_checkalmostequal ( computed , expected , %eps );

// 2.5 Compute second derivative at the same time
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0 1.0 0];
[Jcomputed , Hcomputed] = derivative(myfunction2,x);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 2.6 Test order 1
[Jcomputed , Hcomputed] = derivative(myfunction2,x,order=1);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-8 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-5 );
// 2.7 Test order 2
[Jcomputed , Hcomputed] = derivative(myfunction2,x,order=2);
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 2.8 Test order 4
[Jcomputed , Hcomputed] = derivative(myfunction2,x,order=4);
assert_checkalmostequal ( Jcomputed , Jexpected , %eps );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-10, 1.e-10 );

// 3. Test H_form
// 3.1 Test H_form="default"
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0 1.0 0.0];
[Jcomputed , Hcomputed] = derivative(myfunction2 , x , H_form="default");
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 3.2 Test H_form='hypermat'
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0
1.0 0.0];
[Jcomputed , Hcomputed] = derivative(myfunction2 , x , H_form='hypermat');
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );
// 3.3 Test H_form='hypermat'
Jexpected = [4.0 2.0];
Hexpected = [2.0 1.0 
1.0 0.0];
[Jcomputed , Hcomputed] = derivative(myfunction2 , x , H_form='hypermat');
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-10 );
assert_checkalmostequal ( Hcomputed , Hexpected , %eps );

// 4. Test verbose
[Jcomputed , Hcomputed] = derivative(myfunction2 , x , verbose = 1);

// 5. Test h parameter
// Test a case where the default step h is very small ~ 1.e-9,
// but, because the function is very flat in the neighbourhood of the 
// point, a larger step ~ 1.e-4 reduces the error.
// This means that this test cannot pass if the right step is 
// not taken into account, therefore testing the feature "h is used correctly".
myn = 1.e5;
function y = myfunction3 (x)
  y = x^(2/myn);
endfunction
x = 1.0;
h = 6.055454e-006
Jexpected = (2/myn) * x^(2/myn-1);
Hexpected = (2/myn) * (2/myn-1) * x^(2/myn-2);
[Jcomputed , Hcomputed] = derivative(myfunction3 , x , h = 1.e-4 , order = 1 );
assert_checkalmostequal ( Jcomputed , Jexpected , 1.e-4 );
assert_checkalmostequal ( Hcomputed , Hexpected , 1.e-3 );

// 6. Test Q parameter
// TODO !

