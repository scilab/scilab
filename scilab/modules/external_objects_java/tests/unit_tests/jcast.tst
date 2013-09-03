//
//  Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file must be used under the terms of the CeCILL.
//  This source file is licensed as described in the file COPYING, which
//  you should have received as part of this distribution.  The terms
//  are also available at
//  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
// <-- JVM MANDATORY -->
//

jimport java.lang.Double;
jimport java.lang.Number;

d = Double.new("1.23456");
assert_checkequal("java.lang.Double",jgetclassname(d));
e = jcast(d, "java.lang.Number");
f = jcast(d, Number);
assert_checkequal("java.lang.Number",jgetclassname(e));
jremove e d f;
