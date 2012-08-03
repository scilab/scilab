// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9034 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9034
//
// <-- Short Description -->
// atomsVersionCompare failed on version with '-0'

load("SCI/modules/atoms/macros/atoms_internals/lib");

if atomsVersionCompare("1.46-0"      ,"1.46") <> 0 then pause, end

if atomsVersionCompare("1.4-1"      ,"1.4")       <>  1 then pause, end
if atomsVersionCompare("1.4-1"      ,"1.4-3")     <> -1 then pause, end
if atomsVersionCompare("1.4-10"     ,"1.4-3")     <>  1 then pause, end
if atomsVersionCompare("1.5-1"      ,"1.4-3")     <>  1 then pause, end


A = ["1.4-1";"1.4-3";"1.4.0-2";"1.4-10";"1.5-2";"1.4"];
B = [   -1  ;   1   ;    0    ;    1   ;   1   ;  -1 ];

if or( atomsVersionCompare(A,"1.4-2")<>B ) then pause, end
