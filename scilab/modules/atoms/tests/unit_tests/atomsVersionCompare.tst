// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
load("SCI/modules/atoms/macros/atoms_internals/lib");

if atomsVersionCompare("5.1"        ,"5.1.0")     <> 0 then pause, end
if atomsVersionCompare("5.1.0"      ,"5.1")       <> 0 then pause, end
if atomsVersionCompare("5.1.00000"  ,"5.1.0.0")   <> 0 then pause, end
if atomsVersionCompare("5.1.0.0"    ,"5.1.00000") <> 0 then pause, end

if atomsVersionCompare("5.1.0","5.1.2") <> -1 then pause, end
if atomsVersionCompare("5.1.2","5.1.0") <>  1 then pause, end

if atomsVersionCompare("10.0","2.6.9") <> 1 then pause, end

A = ["5.1";"5.1.0";"5.1.00000";"5.1.0.0";"05.1"];
B = [0 ; 0 ; 0 ; 0 ; 0 ];

if or( atomsVersionCompare(A,"5.1")<>B ) then pause, end

if atomsVersionCompare("1.4-1"      ,"1.4")       <>  1 then pause, end
if atomsVersionCompare("1.4-1"      ,"1.4-3")     <> -1 then pause, end
if atomsVersionCompare("1.4-10"     ,"1.4-3")     <>  1 then pause, end
if atomsVersionCompare("1.5-1"      ,"1.4-3")     <>  1 then pause, end


A = ["1.4-1";"1.4-3";"1.4.0-2";"1.4-10";"1.5-2";"1.4"];
B = [   -1  ;   1   ;    0    ;    1   ;   1   ;  -1 ];

if or( atomsVersionCompare(A,"1.4-2")<>B ) then pause, end
