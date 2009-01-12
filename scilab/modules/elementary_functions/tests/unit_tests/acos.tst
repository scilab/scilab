// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// unit tests for acos() function (element wise cosine inverse)
// =============================================================================

// 1. Interface
// ============

if execstr("acos()"   ,"errcatch") == 0 then pause, end
if execstr("acos(1,2)","errcatch") == 0 then pause, end


// 2. Singular Values
// ==================

rt2 = sqrt (2);
rt3 = sqrt (3);
v   = [0, %pi/6 , %pi/4 , %pi/3, %pi/2 , 2*%pi/3 , 3*%pi/4 , 5*%pi/6 , %pi];
x   = [1, rt3/2 , rt2/2 , 1/2  , 0     , -1/2    , -rt2/2  , -rt3/2  , -1];

if or(abs (acos (x) - v) > sqrt (%eps)) then pause, end


// 3. Not A Number
// ===============

if ~isnan(acos(%nan)) then pause, end
if ~isnan(acos(-%nan)) then pause, end


// 4. Limit values
// ===============

if real(acos(%inf)) <> 0    then pause, end
if imag(acos(%inf)) <> %inf then pause, end

if real(acos(-%inf)) <> %pi  then pause, end
if imag(acos(-%inf)) <> -%inf then pause, end


// 5. Properties
// =============

// acos(-x) = asin(x) + pi/2
A = rand(1000,1000);
if or( acos(-A) - asin(A) - %pi/2 > 2*%eps ) then pause, end
