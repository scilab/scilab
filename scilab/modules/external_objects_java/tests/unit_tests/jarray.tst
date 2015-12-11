// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a = jarray("java.lang.String", 2, 2, 3);
str = "Hi Jims again !!"
// array insertion
a(1, 1, 3) = "Hi Jims !";
a(2, 1, 3) = str;

assert_checkequal(length(str), length(a(2, 1, 3)));
assert_checkequal(str, a(2, 1, 3));
assert_checkequal(a(1), ["" ""; "" ""; "Hi Jims !" ""]);

a = jarray("int", 1);
a(1) = 2;

assert_checkequal(2, double(a(1)));
