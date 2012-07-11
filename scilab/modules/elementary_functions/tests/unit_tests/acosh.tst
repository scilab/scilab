// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for acos() function (element wise cosine inverse)
// =============================================================================

if execstr("acosh()"   ,"errcatch") == 0 then pause, end
if execstr("acosh(1,2)","errcatch") == 0 then pause, end

v = [0, %pi/2*%i , %pi*%i , %pi/2*%i];
x = [1, 0        , -1     , 0       ];

if or(abs(acosh(x)-v) > sqrt(%eps)) then pause, end

if ~isnan(acosh(%nan))        then pause, end
if ~isnan(acosh(-%nan))       then pause, end

if real(acosh(%inf)) <> %inf  then pause, end
if imag(acosh(%inf)) <> 0     then pause, end

if real(acosh(-%inf)) <> %inf then pause, end
if imag(acosh(-%inf)) <> %pi  then pause, end
