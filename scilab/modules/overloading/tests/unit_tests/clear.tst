// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

global cleared_b_called

a = mlist(["my_type" "name"], "a");
b = mlist(["my_type" "name"], "b");

// %my_type_clear() will be called with "b"
function %my_type_clear(v)
    global cleared_b_called
    assert_checkequal(cleared_b_called, [])
    cleared_b_called = 1
    assert_checkequal(v.name, "b")
endfunction
clear b
assert_checkequal(cleared_b_called, 1)

function foo1()
    c = mlist(["my_type" "name"], "c");
    // %my_type_clear() will be called with "c"
endfunction
function %my_type_clear(v)
    assert_checkequal(v.name, "c")
endfunction
foo1()

function foo2()
    ac = a;
    // %my_type_clear() will not be called
endfunction
function %my_type_clear(v)
    pause
endfunction
foo2()

// aa is a reference to a containing the same data
aa = a;

// %my_type_clear() will be called with "a" only once
function %my_type_clear(v)
    global cleared_a_called
    assert_checkequal(cleared_a_called, [])
    cleared_a_called = 1
    assert_checkequal(v.name, "a")
endfunction
clear aa
clear a


