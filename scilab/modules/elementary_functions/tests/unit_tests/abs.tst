// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for abs() function (absolute value, magnitude)
// =============================================================================

if execstr("abs()"   ,"errcatch") == 0 then pause, end
if execstr("abs(1,2)","errcatch") == 0 then pause, end

if abs(1)      <> 1   then pause, end
if abs(-3.5)   <> 3.5 then pause, end
if abs(3+4*%i) <> 5   then pause, end
if abs(3-4*%i) <> 5   then pause, end

if or(abs([ 1.1 , 3*%i ; 3+4*%i 3-4*%i ])   <> [1.1 , 3 ; 5 , 5 ]) then pause, end
if or(abs([ 1.1 3*%i 3+4*%i 3-4*%i ])       <> [1.1 3 5 5 ])       then pause, end
if or(abs([ 1.1 ; 3*%i ; 3+4*%i ; 3-4*%i ]) <> [1.1 ; 3 ; 5 ; 5 ]) then pause, end


if ~isnan(abs(%nan)) then pause, end
if ~isnan(abs(-%nan)) then pause, end

if abs(%inf) <> %inf then pause, end
if abs(-%inf) <> %inf then pause, end
