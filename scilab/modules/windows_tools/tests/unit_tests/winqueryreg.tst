// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- WINDOWS ONLY -->


assert_checkerror('winqueryreg()', [], 77);
assert_checkerror('winqueryreg(''name'', ''HKEY_CURRENT_USER'')', [], 999);

mousechar = winqueryreg('name', 'HKEY_CURRENT_USER','control panel\mouse');
assert_checktrue(size(mousechar,'*') <> 0);

cpu = winqueryreg('HKEY_LOCAL_MACHINE','HARDWARE\DESCRIPTION\System\CentralProcessor\0\','ProcessorNameString');
assert_checktrue(length(cpu) <> 0);

software1 = winqueryreg('key', 'HKEY_LOCAL_MACHINE','Software');
assert_checktrue(size(software1, "*") <> 0);

software2 = winqueryreg('key', 'HKLM','Software');
assert_checktrue(size(software2, "*") <> 0);

assert_checkequal(software1, software2);
