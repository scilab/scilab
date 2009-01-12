// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// unit tests for asin() function (element wise coasine inverse)
// =============================================================================


// 1. Interface
// ============

if execstr("asin()"   ,"errcatch") == 0 then pause, end
if execstr("asin(1,2)","errcatch") == 0 then pause, end


// 2. Singular Values
// ==================

rt2 = sqrt(2);
rt3 = sqrt(3);
v   = [0 , %pi/6 , %pi/4 , %pi/3 , %pi/2    , %pi/3 , %pi/4 , %pi/6 , 0 ];
x   = [0 , 1/2   , rt2/2 , rt3/2 , 1        , rt3/2 , rt2/2 , 1/2   , 0 ];

if or(abs(asin(x)-v) > sqrt (%eps)) then pause, end


// 3. Not A Number
// ===============

if ~isnan(asin(%nan))        then pause, end
if ~isnan(asin(-%nan))       then pause, end


// 4. Limit values
// ===============

// 5. Properties
// =============

A = rand(100,100);

// asin(-x) = - asin(x)
if or( asin(-A) + asin(A) > %eps) then pause, end


// asin(x) = acos(-x) - pi/2
if or( (asin(A) - acos(-A) + %pi/2) > %eps ) then pause, end
