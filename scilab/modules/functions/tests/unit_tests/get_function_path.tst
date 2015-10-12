// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for get_function_path() function
// =============================================================================

assert_checkequal(get_function_path('abcd'), pathconvert(getlongpathname(SCI+'/modules/cacsd/macros/abcd.sci'), %f));
assert_checkequal(get_function_path('dae'), pathconvert(getlongpathname(SCI+'/modules/differential_equations/macros/dae.sci'), %f));
assert_checkequal(get_function_path('acosd'), pathconvert(getlongpathname(SCI+'/modules/elementary_functions/macros/acosd.sci'), %f));
assert_checkequal(get_function_path('dir'), pathconvert(getlongpathname(SCI+'/modules/fileio/macros/dir.sci'), %f));
assert_checkequal(get_function_path('get_function_path'), pathconvert(getlongpathname(SCI+'/modules/functions/macros/get_function_path.sci'), %f));
assert_checkequal(get_function_path('get_param'), pathconvert(getlongpathname(SCI+'/modules/parameters/macros/get_param.sci'), %f));
assert_checkequal(get_function_path('median'), pathconvert(getlongpathname(SCI+'/modules/statistics/macros/median.sci'), %f));
assert_checkequal(get_function_path('calendar'), pathconvert(getlongpathname(SCI+'/modules/time/macros/calendar.sci'), %f));

assert_checkequal(get_function_path('eigs.sci'),[]);
assert_checkfalse(execstr("get_function_path()","errcatch")==0);
refMsg = msprintf(_("%s: Wrong number of input argument: %d expected.\n"), "get_function_path", 1);
assert_checkerror("get_function_path()", refMsg);
assert_checkfalse(execstr("get_function_path(eigs)","errcatch")==0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"get_function_path",1);
assert_checkerror("get_function_path(eigs)", refMsg);
