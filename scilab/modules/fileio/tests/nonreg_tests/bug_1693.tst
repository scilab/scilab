// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1693 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1693
//
// <-- Short Description -->
//    cd \ throws an error
// ...

// ================== Test 1 ==================

cd('\');

if getos() == 'Windows' 
  
	if ~or(getdrives() == pwd()) then pause,end
else
	if pwd() <> '/' then pause,end
end

// ================== Test 2 ==================

cd home;
if pwd() <> home then pause,end

// ================== Test 3 ==================

if getos() == 'Windows' then
	cd WSCI;
	if pwd() <> WSCI then pause,end
end

// ================== Test 4 ==================

cd SCIHOME;
if pwd() <> SCIHOME then pause,end

// ================== Test 5 ==================

cd SCIHOME;
if pwd() <> SCIHOME then pause,end

// ================== Test 6 ==================

cd PWD;
if pwd() <> PWD then pause,end
