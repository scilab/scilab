// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

assert_checktrue(execstr("system_setproperty();","errcatch")==999);

assert_checktrue(execstr("system_getproperty();","errcatch")==999);

assert_checktrue(execstr("a = system_setproperty(""TOTO"",2);","errcatch")==999);

r = system_getproperty("TOTO");
assert_checkequal(r, "unknown");

assert_checktrue(execstr("a = system_setproperty(""TOTO"",""TATA"");","errcatch")==0);
assert_checkequal(a, []);
r = system_getproperty("TOTO");
assert_checkequal(r, "TATA");

assert_checktrue(execstr("a = system_setproperty(""TOTO"",""TITI"");","errcatch")==0);
assert_checkequal(a, "TATA");
r = system_getproperty("TOTO");
assert_checkequal(r, "TITI");
