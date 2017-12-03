// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//who

a    = 33;
bbb  = -1;
vars = who("get");
assert_checktrue(find(vars == "bbb") <> []);
assert_checktrue(find(vars == "a") <> []);

function foo(a, b, c)
    lst = who("scope");
    //remove nargin/nargout
    lst(find(lst == "nargin")) = [];
    lst(find(lst == "nargout")) = [];

    //check args against who
    assert_checkequal(setdiff(lst, a), []);
endfunction

//normal argument passing
foo(["a"]);
foo(["a";"b"], 1);
foo(["a";"b";"c"], 1, 2);

//named existing arguments
foo(a = ["a"]);
foo(a = ["a";"b"], b = 1);
foo(b = 1, a = ["a";"b"]);
foo(a = ["a";"c"], c = 2);
foo(c = 2, a = ["a";"c"], c = 1);
foo(a = ["a";"b";"c"], b = 1, c = 2);
foo(a = ["a";"b";"c"], c = 2, b = 1);
foo(b = 1, a = ["a";"b";"c"], c = 2);
foo(b = 1, c = 2, a = ["a";"b";"c"]);
foo(c = 2, a = ["a";"b";"c"], b = 1);
foo(c = 2, b = 1, a = ["a";"b";"c"]);

//named non existing arguments
foo(a = ["a";"g"], g = 1);
foo(b = 1, a = ["a";"b"]);
foo(a = ["a";"h"], h = 1);
foo(h = 1, a = ["a";"h"]);
