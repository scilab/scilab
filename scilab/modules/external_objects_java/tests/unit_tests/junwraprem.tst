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

jimport java.lang.String;

s = String.new("Hello JIMS !!");
s1 = s.toUpperCase();
s2 = s.toLowerCase();
[S1 S2] = junwraprem(jwrap(s1), jwrap(s2));
jremove s

assert_checkequal(S1,"HELLO JIMS !!");
assert_checkequal(S2,"hello jims !!");
