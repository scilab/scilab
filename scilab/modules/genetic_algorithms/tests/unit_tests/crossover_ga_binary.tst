
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

[Crossed_Indiv1, Crossed_Indiv2] = crossover_ga_binary('11111111','00000000',[]);

if (length(Crossed_Indiv1)~=8) | (length(Crossed_Indiv2)~=8) then pause,end

//==============================================================================
// Nominal Behaviour
//==============================================================================
param = init_param("binary_length", 8, "multi_cross", %F, "multi_cross_nb", 1);
A = "11111111";
B = "00000000";

// Reinitialize the seed
grand("setsd", 0);
[A_crossed, B_crossed, mix] = crossover_ga_binary(A, B);

// Reinitialize the seed
grand("setsd", 0);
[A_crossed_p, B_crossed_p, mix] = crossover_ga_binary(A, B, param);

//======================================
// Check default behaviour of param
//======================================
assert_checkequal(A_crossed_p, A_crossed);
assert_checkequal(B_crossed_p, B_crossed);
assert_checkequal(size(mix, "*"), 1);

//======================================
// Check the crossover occurred
//======================================
// Heads
assert_checkequal(part(A, 1:mix), part(A_crossed, 1:mix));
assert_checkequal(part(B, 1:mix), part(B_crossed, 1:mix));
// Tails
assert_checkequal(part(A, (mix + 1):$), part(B_crossed, (mix + 1):$));
assert_checkequal(part(B, (mix + 1):$), part(A_crossed, (mix + 1):$));

//======================================
// Binary length
//======================================
param = set_param(param, "binary_length", 16);
A = dec2bin(2^16 - 1, 16); // 11111111 11111111
B = dec2bin(0, 16); // 00000000 00000000

[A_crossed, B_crossed, mix] = crossover_ga_binary(A, B, param);
assert_checkequal(length(A_crossed), 16);
assert_checkequal(length(B_crossed), 16);

//======================================
// Multiple Crossover
//======================================
param = init_param("binary_length", 16, "multi_cross", %T);
s_mix = 0;
iter = 0;
// By default 2 crossovers
while s_mix < 2 & iter <= 100
    [A_crossed, B_crossed, mix] = crossover_ga_binary(A, B, param);
    s_mix = size(mix, "*");
    iter = iter + 1;
end

// Warning probabilistic test
assert_checktrue(s_mix == 2); // we have reached one mix of length 2
assert_checkfalse(iter>=100); // we have done it under 100 tries
assert_checkequal(part(A_crossed, 1:mix(1)), part(A, 1:mix(1)));
assert_checkequal(part(B_crossed, 1:mix(1)), part(B, 1:mix(1)));

// Swapped
assert_checkequal(part(A_crossed, (mix(1) + 1):mix(2)), part(B, (mix(1) + 1):mix(2)));
assert_checkequal(part(B_crossed, (mix(1) + 1):mix(2)), part(A, (mix(1) + 1):mix(2)));

// Same
assert_checkequal(part(A_crossed, (mix(2) + 1):$), part(A, (mix(2) + 1):$));
assert_checkequal(part(B_crossed, (mix(2) + 1):$), part(B, (mix(2) + 1):$));

//======================================
// Multiple Crossover Number
//======================================
param = init_param("binary_length", 16, "multi_cross", %T, "multi_cross_nb", 4);
s_mix = 0;
iter = 0;
// By default 2 crossovers
while s_mix < 4 & iter <= 100
    [A_crossed, B_crossed, mix] = crossover_ga_binary(A, B, param);
    s_mix = size(mix, "*");
    iter = iter + 1;
end

// Warning probabilistic test
assert_checktrue(s_mix == 4); // we have reached one mix of length 4
assert_checkfalse(iter>=100); // we have done it under 100 tries
