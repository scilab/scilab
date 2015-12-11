// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport java.lang.String;

s = String.new("Hello JIMS !!");
s1 = s.toUpperCase();
assert_checkequal(s1,"HELLO JIMS !!");
s2 = s.toLowerCase();
assert_checkequal(s2,"hello jims !!");
[S1 S2] = junwrap(jwrap(s1), jwrap(s2));

assert_checkequal(S1,"HELLO JIMS !!");
assert_checkequal(S2,"hello jims !!");
