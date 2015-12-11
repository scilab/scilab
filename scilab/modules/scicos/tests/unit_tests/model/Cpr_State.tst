// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Allocate a State and check its default values
state = scicos_state();

header = ["xcs" "x" "z" "oz" "iz" "tevts" "evtspt" "pointi" "outtb"];

assert_checkequal(state(1), header);
for i=2:7
    assert_checkequal(state(i), []);
end
assert_checkequal(state(8), 1);
assert_checkequal(state(9), list());


// Allocate a Cpr and check its default values
cpr = scicos_cpr();

header = ["cpr" "state" "sim" "cor" "corinv"];

assert_checkequal(cpr(1), header);
assert_checkequal(cpr(2), state);
assert_checkequal(cpr(3), scicos_sim());
assert_checkequal(cpr(4), list());
assert_checkequal(cpr(5), list());
