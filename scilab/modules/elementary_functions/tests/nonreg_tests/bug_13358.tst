// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 13358-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13358
//
// <-- Short Description -->
// intersect and unique are slower due to gsort behaving in o(n^2) on sorted arrays
//
// <-- INTERACTIVE TEST -->
// <-- CLI SHELL MODE -->

err = 0.15
seed = getdate("s");
rand("seed", seed);
nb_test = 5;
A = 1:1E5;
B = 2:2:2E5;
delta_i = [];
delta_u = [];
delta_s = [];

// Checks relative time between sort on a random table and a sorted one is within 15%
for i = 1:nb_test
    A_rand = rand(1, 1E5);
    B_rand = rand(1, 1E5);
    tic(); intersect(A, B); t_elapsed_sorted = toc();
    tic(); intersect(A_rand, B_rand); t_elapsed_rand = toc();
    delta_i = [delta_i, abs(t_elapsed_rand - t_elapsed_sorted) / (t_elapsed_rand + t_elapsed_sorted)];

    tic(); unique(A); t_elapsed_sorted = toc();
    tic(); unique(A_rand); t_elapsed_rand = toc();
    delta_u = [delta_u, abs(t_elapsed_rand - t_elapsed_sorted) / (t_elapsed_rand + t_elapsed_sorted)];

    tic(); gsort(1:1E6); t_elapsed_sorted = toc()
    tic(); gsort(rand(1,1E6)); t_elapsed_rand = toc()
    delta_s = [delta_s, abs(t_elapsed_rand - t_elapsed_sorted) / (t_elapsed_rand + t_elapsed_sorted)];
end

assert_checktrue(mean(delta_i) <= err);
assert_checktrue(mean(delta_u) <= err);
assert_checktrue(mean(delta_s) <= err);
