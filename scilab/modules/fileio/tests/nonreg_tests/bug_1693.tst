// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1693 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1693
//
// <-- Short Description -->
//    cd \ throws an error
//
//    -->cd \
//     !--error 998
//    Can't go to directory \
//    at line       6 of function cd called by :
//    cd \
//
//    In scilab-3.1.1, it works
//
//    -->cd \
//     ans  =
//
//     C:\
//
//    This was reported on the newsgroup:
//    http://groups.google.fr/group/comp.soft-
//    sys.math.scilab/browse_thread/thread/3e2f8a72af0148fd/e0f7866780de7fbf
//
//    Since the Windows command interpreter accepts this (cd \ changes 
// ...

// ================== Test 1 ==================

cd \

if MSDOS 
	if pwd() <> 'C:\' then pause,end
else
	if pwd() <> '/' then pause,end
end

// ================== Test 2 ==================

cd home;
if pwd() <> home then pause,end

// ================== Test 3 ==================

if MSDOS then
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
