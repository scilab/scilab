// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================

s = "Hello World !";
r = jgetfield(jwrap(s), "CASE_INSENSITIVE_ORDER");

c = jcompile("Test", ["public class Test {";
"public int field;";
"public Test(int n) {";
"field = n;";
"}";
"}";]);
t = c.new(128);
jsetfield(t, "field", 256);
assert_checkequal(int32(256), jgetfield(t, "field"));