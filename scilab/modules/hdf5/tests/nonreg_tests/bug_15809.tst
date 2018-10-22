// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15809 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15809
//
// <-- Short Description -->
// HDF5 load/save was super slow for nested lists


N = 4;
n = 1000;
filters_sys = list();
filters_vec = list();
for i=1:n
    G=syslin("c", rand(N,N), rand(N,1), rand(1,N), rand(1,1));
    filters_sys($+1) = G;
    filters_vec($+1) = [G.a G.b;G.c G.d];
end

tic();
save("TMPDIR/filters_sys.sod", "filters_sys");
sys = toc();
tic();
save("TMPDIR/filters_vec.sod", "filters_vec");
vec = toc();

assert_checkfalse(sys > 15 * vec);


