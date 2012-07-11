// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3561 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3561
//
// <-- Short Description -->
// My DLL "C" functions returning character strings do not work with SciLab 5.0.x.

ilib_verbose(0);

if getos() == 'Windows' then

currentpath = pwd();
cd TMPDIR;

mkdir(TMPDIR,'bug_3561');
TEST_DIR = TMPDIR + filesep() + 'bug_3561';

copyfile(SCI+'/modules/dynamic_link/tests/nonreg_tests/bug3561Dll.h' , TEST_DIR + filesep() + 'bug3561Dll.h');
copyfile(SCI+'/modules/dynamic_link/tests/nonreg_tests/bug3561Dll.cpp' , TEST_DIR + filesep() + 'bug3561Dll.cpp');

chdir(TEST_DIR);

files=['bug3561Dll.cpp'];
ilib_for_link('bug_3561',['bug3561Dll.cpp'],[],'cpp');
functions = ['Add', 'WriteString'];
link('libbug_3561.dll', functions, 'c');

rc = call('WriteString', 'out', [5,1], 1, 'c');
if type(rc) <> 10 then pause,end
if length(rc)<> 5 then pause,end
if rc<>'12345' then pause,end
end
