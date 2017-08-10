//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Test 1
// =============================================================================

if execstr("getversion();","errcatch")                                 <> 0 then pause, end
if execstr("[version,opts]=getversion()","errcatch")                   <> 0 then pause, end
if execstr("my_version=getversion(''scilab'')","errcatch")             <> 0 then pause, end
if execstr("verstr=getversion(''scilab'',''string_info'')","errcatch") <> 0 then pause, end

module_list = getmodules();
for i=1:size(module_list,"*")
	if execstr("my_version=getversion(''"+module_list(i)+"'')","errcatch")             <> 0 then pause, end
	if execstr("verstr=getversion(''"+module_list(i)+"'',''string_info'')","errcatch") <> 0 then pause, end
end

// Test 2
// =============================================================================

// Test 2.1

A = getversion();
if type(A) <> 10 then pause, end

// Test 2.2
[A,B] = getversion();
if type(A) <> 10 then pause, end
if type(B) <> 10 then pause, end

// Test 2.3
A = getversion("scilab");
if type(A) <> 1          then pause, end
if or( size(A) <> [1 4]) then pause, end

// Test 2.4
A = getversion("scilab","string_info");
if type(A) <> 10 then pause, end

// Test 2.5
module_list = getmodules();
for i=1:size(module_list,"*")
	A = getversion(module_list(i));
	B = getversion(module_list(i),"string_info");
	if type(A) <> 1 then pause, end
	if type(B) <> 10 then pause, end
	if or( size(B) <> [1 1]) then pause, end
end
