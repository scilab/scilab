// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for sinh() function (element wise sinus inverse)
// =============================================================================

// 1. Interface
// ============

if execstr("sinh()"   ,"errcatch")           == 0 then pause, end
if execstr("sinh(1,2)","errcatch")           == 0 then pause, end
if execstr("sinh(''my string'')","errcatch") == 0 then pause, end


// 2. Singular Values
// ==================

x = [0, %pi/2*%i , %pi*%i , 3*%pi/2*%i ];
v = [0, %i       , 0      , -%i        ];

if or(abs(sinh(x)-v) > sqrt (%eps)) then pause, end


// 3. Not A Number
// ===============

if ~isnan(sinh(%nan))        then pause, end
if ~isnan(sinh(-%nan))       then pause, end


// 4. Limit values
// ===============

if real(sinh(%inf)) <> %inf  then pause, end
if imag(sinh(%inf)) <> 0     then pause, end

if real(sinh(-%inf)) <> - %inf then pause, end
if imag(sinh(-%inf)) <> 0      then pause, end


// 5. Properties
// =============

A = rand(100,100);

// sinh(i * x) = i * sin(x)
if or( abs(sinh(%i*A) - (%i * sin(A))) > 2*%eps) then pause, end

// sinh^2(x/2) = (cosh(x)-1)/2
if or( abs( (sinh(A/2).^2) - ((cosh(A)-1)/2) ) > 2*%eps) then pause, end
