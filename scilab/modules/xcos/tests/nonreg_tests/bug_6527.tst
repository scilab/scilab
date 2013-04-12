// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE <yann.collette@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->

// <-- Non-regression test for bug 6527 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6527
//
// <-- Short Description -->
// multiple instance of the same variable when exporting via "to workspace"


importXcosDiagram(SCI+"/modules/xcos/tests/nonreg_tests/bug_6527.zcos");

Info = scicos_simulate(scs_m,list());

names = who('local');
nb_var = length(find(names=="A_AND"));

if nb_var~=1 then pause; end
