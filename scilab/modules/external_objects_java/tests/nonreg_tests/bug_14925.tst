// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14925 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14925
//
// <-- Short Description -->
// jdeff(): invoke_lu() deleted/corrupted the called java method when an error occurred
//

jdeff("java.lang.Double", "parseDouble", "str2number");
PI = str2number("3.14");
try
    PI = str2number(3.14);
end
clear PI
assert_checktrue(execstr("PI = str2number(""3.14"")", "errcatch")==0);
assert_checkequal(PI, 3.14);
