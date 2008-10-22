// Non-regression test file for bug 1505
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 28 décembre 2005

mode(-1);
clear


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

clear
