// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 10695 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10695
//
// <-- Short Description -->
// scf() and plot() could fail.

cd("SCI/modules/graphics/tests/nonreg_tests");

load ("betas.dat");

scf;
plot(betas(:, 1), betas(:, 3), ".");
