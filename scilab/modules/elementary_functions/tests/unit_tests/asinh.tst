// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for asinh() function (element wise coasine inverse)
// =============================================================================

if execstr("asinh()"   ,"errcatch") == 0 then pause, end
if execstr("asinh(1,2)","errcatch") == 0 then pause, end

x = [0, (%pi/2) * %i , 0  , -(%pi/2) * %i ];
v = [0, %i           , 0  , -%i           ];

if or(abs(asinh(v)-x) > sqrt(%eps)) then pause, end

if ~isnan(asinh(%nan))        then pause, end
if ~isnan(asinh(-%nan))       then pause, end

if real(asinh(%inf)) <> %inf  then pause, end
if imag(asinh(%inf)) <> 0     then pause, end

if real(asinh(-%inf)) <> - %inf then pause, end
if imag(asinh(-%inf)) <> 0      then pause, end
