//
//  Copyright (C) 2011 - DIGITEO - Allan CORNET
//  Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file must be used under the terms of the CeCILL.
//  This source file is licensed as described in the file COPYING, which
//  you should have received as part of this distribution.  The terms
//  are also available at
//  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
// to ignore java exceptions in last test
//<-- NO CHECK ERROR OUTPUT -->
//
// <-- JVM MANDATORY -->
//

s = jwrap("Hello World !");
assert_checkequal(jgetclassname(s), "java.lang.String");
jremove s;

assert_checkequal(jgetclassname("toto"), "java.lang.String");
assert_checkequal(jgetclassname(1), "double");
assert_checkequal(jgetclassname(%t), "boolean");
assert_checkequal(jgetclassname(int8(2)), "byte");
assert_checkequal(jgetclassname(int16(2)), "short");
assert_checkequal(jgetclassname(int32(2)), "int");

s = scf();
assert_checkequal(execstr("jgetclassname(s)", "errcatch"), 999);


