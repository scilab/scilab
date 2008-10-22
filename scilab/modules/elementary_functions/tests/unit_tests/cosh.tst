// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for cosh() function (element wise cosine inverse)
// =============================================================================


// 1. Interface
// ============

if execstr("cosh()"   ,"errcatch")           == 0 then pause, end
if execstr("cosh(1,2)","errcatch")           == 0 then pause, end
if execstr("cosh(''my string'')","errcatch") == 0 then pause, end


// 2. Singular Values
// ==================

x = [0, %pi/2*%i , %pi*%i , 3*%pi/2*%i ];
v = [1, 0        , -1     , 0          ];

if or(abs(cosh(x)-v) > sqrt (%eps)) then pause, end


// 3. Not A Number
// ===============

if ~isnan(cosh(%nan))   then pause, end
if ~isnan(cosh(-%nan))  then pause, end


// 4. Limit values
// ===============

if cosh(%inf)  <> %inf  then pause, end
if cosh(-%inf) <> %inf  then pause, end


// 5. Properties
// =============

A = rand(100,100);

// cosh(i * x) = cos(x)
if or( abs(cosh( %i * A ) - cos(A)) > 2*%eps) then pause, end

// cosh^2(x/2) = (1 + cosh(x)) / 2
if or( abs( (cosh(A/2)).^2 - ( (1 + cosh(A))/2 ) ) > 2*%eps) then pause, end
