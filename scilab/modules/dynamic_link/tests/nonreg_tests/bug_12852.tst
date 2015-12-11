// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- WINDOWS ONLY -->
//
// <-- Non-regression test for bug 12852 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12852
//
// <-- Short Description -->
// Visual Studio 2012 SDK configuration is incorrect.

ilib_verbose(0);
curPath = pwd();
cd TMPDIR;
src = ['#include <windows.h>'
    'int create_directory(char* path)'
    '{'
    '   return CreateDirectory(path, NULL);';
    '}'];
srcfile = 'test_windows_sdk.c';
mputl(src, srcfile);

libn = ilib_for_link('test_windows_sdk', srcfile, [], "c");
assert_checktrue(libn <> 0);

exec('cleaner.sce');
cd(curPath);
