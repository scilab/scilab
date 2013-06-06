//
//  Copyright (C) 2011 - DIGITEO - Allan CORNET
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

s = jwrap("Hello World !");
if jgetclassname(s) <> "java.lang.String" then pause, end
jremove s;

if jgetclassname("toto") <> "java.lang.String" then pause, end

if jgetclassname(1) <> "double" then pause, end

if jgetclassname(%t) <> "boolean" then pause, end

if jgetclassname(int8(2)) <> "char" then pause, end

if jgetclassname(int16(2)) <> "short" then pause, end

if jgetclassname(int32(2)) <> "int" then pause, end

s = scf();
if execstr("jgetclassname(s)", "errcatch") <> 999, end


