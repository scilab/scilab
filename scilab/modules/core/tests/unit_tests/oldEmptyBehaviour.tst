// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
//
// <-- ENGLISH ONLY -->

// Testing oldEmptyBehaviour default mode
behaviour = oldEmptyBehaviour("query");
assert_checkequal(behaviour, "off");

// Changing the behaviour
oldEmptyBehaviour("on");
behaviour = oldEmptyBehaviour("query");
assert_checkequal(behaviour, "on");
E = [];
s = 1;
sc = 1 + %i;
M = [1 2; 3 4];
MC = M + M.*%i;
I = int8(M);
P = [3*%s^2 + %s + 1; %s^2];
PC = P + %i*%s;
st = ["this is a test"; "with a string"];

num = {s, sc, M, MC, I, P, PC};

// Test all numeric
for i = 1:size(num, "*")
    assert_checkequal(E + num{i}, num{i});
    assert_checkequal(E - num{i}, -num{i});
    assert_checkequal(num{i} + E, num{i});
    assert_checkequal(num{i} - E, num{i});
end

// test strings
assert_checkequal(E + st, st);
assert_checkequal(st + E, st);

// New behaviour
oldEmptyBehaviour("off");
behaviour = oldEmptyBehaviour("query");
assert_checkequal(behaviour, "off");

// Test all numeric
for i = 1:size(num, "*")
    assert_checkequal(E + num{i}, E);
    assert_checkequal(E - num{i}, E);
    assert_checkequal(num{i} + E, E);
    assert_checkequal(num{i} - E, E);
end

// test strings
assert_checkequal(E + st, E);
assert_checkequal(st + E, E);

