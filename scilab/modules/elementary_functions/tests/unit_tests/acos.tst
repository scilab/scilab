// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2011 - DIGITEO - Michaël Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for acos() function (element wise cosine inverse)
// =============================================================================

// 1. Interface
// ============

assert_checkfalse(execstr("acos()"   ,"errcatch") == 0);
assert_checkfalse(execstr("acos(1,2)","errcatch") == 0);


// 2. Singular Values
// ==================

rt2 = sqrt (2);
rt3 = sqrt (3);
x   = [1, rt3/2 , rt2/2 , 1/2  , +0    , -0,    -1/2    , -rt2/2  , -rt3/2  , -1];
v   = [0, %pi/6 , %pi/4 , %pi/3, %pi/2 , %pi/2, 2*%pi/3 , 3*%pi/4 , 5*%pi/6 , %pi];

y = acos(x);
assert_checkalmostequal(y,v,2*%eps,[],"element");

assert_checkequal(acos(1),0);


// 3. Not A Number
// ===============

assert_checkequal(acos(%nan),%nan);
assert_checkequal(acos(-%nan),%nan);


// 4. Limit values
// ===============

assert_checkequal(real(acos(%inf)),0);
assert_checkequal(imag(acos(%inf)),%inf);

assert_checkequal(real(acos(-%inf)),%pi);
assert_checkequal(imag(acos(-%inf)),-%inf);


// 5. Properties
// =============

// acos(-x) = asin(x) + pi/2
n = 100;
A = rand(n,n);
B = acos(-A) - asin(A);
e = %pi/2*ones(n,n);
assert_checkalmostequal(B, e, 4*%eps, [], "element");
