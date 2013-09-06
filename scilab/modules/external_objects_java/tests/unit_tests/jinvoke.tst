// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport java.io.File;
jimport java.lang.System;
jimport java.awt.Color;

a = System.getProperty("java.io.tmpdir");
s = jnewInstance(File, a);
assert_checktrue(jinvoke(s,"exists"));

str = jnewInstance("java.lang.String", "Hello");
assert_checkequal(jinvoke(str,"substring",1, 4), "ell");

assert_checktrue(length(jinvoke(System, "getProperty", "java.version")) > 5);

c = jinvoke(Color, "decode", "123456");
cb = jinvoke(c, "brighter");
assert_checkequal(jgetclassname(cb), "java.awt.Color");

jremove System s str b a;
