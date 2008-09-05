// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for asin() function (element wise coasine inverse)
// =============================================================================

if execstr("asin()"   ,"errcatch") == 0 then pause, end
if execstr("asin(1,2)","errcatch") == 0 then pause, end

x = [0, %pi/2*%i , 0  , -%pi/2*%i ];
v = [0, %i       , 0  , -%i       ];

if or(abs(asin(x)-v) > sqrt (%eps)) then pause, end

if ~isnan(asin(%nan))        then pause, end
if ~isnan(asin(-%nan))       then pause, end

if real(asin(%inf)) <> %inf  then pause, end
if imag(asin(%inf)) <> 0     then pause, end

if real(asin(-%inf)) <> - %inf then pause, end
if imag(asin(-%inf)) <> 0      then pause, end
