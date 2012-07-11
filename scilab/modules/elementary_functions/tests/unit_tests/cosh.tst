// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2011 - DIGITEO - Michaël Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for cosh() function (element wise cosine inverse)
// =============================================================================


// 1. Interface
// ============

if execstr("cosh()"   ,"errcatch")           == 0 then pause, end
if execstr("cosh(1,2)","errcatch")           == 0 then pause, end
if execstr("cosh(''my string'')","errcatch") == 0 then pause, end


// 2. Singular Values
// ==================

x = [-0, +0, %pi/2*%i , %pi*%i , 3*%pi/2*%i ];
v = [ 1,  1, 0        , -1     , 0          ];

y = cosh(x);
assert_checkalmostequal(y,v,%eps,1.e-15,"element");


// 3. Not A Number
// ===============

assert_checkequal(cosh(%nan),%nan);
assert_checkequal(cosh(-%nan),%nan);


// 4. Limit values
// ===============

assert_checkequal(cosh(%inf),%inf);
assert_checkequal(cosh(-%inf),%inf);


// 5. Properties
// =============

A = rand(100,100);

// cosh(i * x) = cos(x)
y1 = cosh( %i * A );
y2 = complex(cos(A));
assert_checkalmostequal(y1,y2,%eps,[],"element");

// cosh^2(x/2) = (1 + cosh(x)) / 2
y1 = (cosh(A/2)).^2;
y2 = (1 + cosh(A))/2;
assert_checkalmostequal(y1,y2,4*%eps,[],"element");
