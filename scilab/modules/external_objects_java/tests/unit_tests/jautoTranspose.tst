//
//  Copyright (C) 2013 - S/E - Sylvestre Ledru
//
//  This file must be used under the terms of the CeCILL.
//  This source file is licensed as described in the file COPYING, which
//  you should have received as part of this distribution.  The terms
//  are also available at
//  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//
// <-- JVM MANDATORY -->
//
warning("off");

jautoTranspose(%t);
assert_checkequal(jautoTranspose(), %t);

assert_checkequal(jconvMatrixMethod(),"rc");

jconvMatrixMethod("cr");
assert_checkequal(jconvMatrixMethod(),"cr");
assert_checkequal(jautoTranspose(), %f);
