// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2848 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2848
//
// <-- Short Description -->
//    When using the TAB key for completion in the console, after using once a Scilab
//    macro, its name appears twice in the completion list: as a macro and as a variable.


[a, b, c, d] = completion("with");
assert_checkequal(c, []);

with_tk();
[a, b, c, d]=completion("with");
assert_checkequal(c, []);

clear with_tk
[a, b, c, d]=completion("with");
assert_checkequal(c, []);

with_tk();

function x = with_foo(y, z)
    x = y + z;
endfunction

[a, b, c, d] = completion("with");
ref_a = ["with_module"];
ref_b = [];
ref_c = [];
ref_d = ["with_foo"; ..
"with_javasci"; ..
"with_macros_source"; ..
"with_modelica_compiler"; ..
"with_tk"];

assert_checkequal(a, ref_a);
assert_checkequal(b, ref_b);
assert_checkequal(c, ref_c);
assert_checkequal(d, ref_d);

clear with_tk
clear with_foo

[a,b,c,d] = completion("with");
assert_checkequal(c, []);
