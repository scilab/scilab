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

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

// ================== Test 1 ==================

cd \

if MSDOS 
	if pwd() <> 'C:\' then
		test1 = %F;
	else
		test1 = %T;
	end
else
	if pwd() <> '/' then
		test1 = %F;
	else
		test1 = %T;
	end
end

// ================== Test 2 ==================

cd home

if pwd() <> home then
	test2 = %F;
else
	test2 = %T;
end

// ================== Test 3 ==================

if MSDOS 
	cd WSCI
	if pwd() <> WSCI then
		test3 = %F;
	else
		test3 = %T;
	end
else
	test3 = %T;
end

// ================== Test 4 ==================

cd SCIHOME

if pwd() <> SCIHOME then
	test4 = %F;
else
	test4 = %T;
end

// ================== Test 5 ==================

cd SCIHOME

if pwd() <> SCIHOME then
	test5 = %F;
else
	test5 = %T;
end

// ================== Test 6 ==================

cd PWD

if pwd() <> PWD then
	test6 = %F;
else
	test6 = %T;
end

// ================== Result ==================

if ( test1 & test2 & test3 & test4 & test5 & test6 ) then
	affich_result(%T,1693);
else
	affich_result(%F,1693);
end
