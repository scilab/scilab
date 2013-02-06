// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for get_function_path() function
// =============================================================================

assert_checkequal(get_function_path('abcd'),SCI+'/modules/cacsd/macros/abcd.sci');
assert_checkequal(get_function_path('dae'),SCI+'/modules/differential_equations/macros/dae.sci');
assert_checkequal(get_function_path('acosd'),SCI+'/modules/elementary_functions/macros/acosd.sci');
assert_checkequal(get_function_path('dir'),SCI+'/modules/fileio/macros/dir.sci');
assert_checkequal(get_function_path('get_function_path'),SCI+'/modules/functions/macros/get_function_path.sci');
assert_checkequal(get_function_path('get_param'),SCI+'/modules/parameters/macros/get_param.sci');
assert_checkequal(get_function_path('median'),SCI+'/modules/statistics/macros/median.sci');
assert_checkequal(get_function_path('calendar'),SCI+'/modules/time/macros/calendar.sci');

assert_checkequal(get_function_path('eigs.sci'),[]);
assert_checkfalse(execstr("get_function_path()","errcatch")==0);
assert_checkfalse(execstr("get_function_path(eigs)","errcatch")==0);
