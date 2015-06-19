// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 INRIA Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 4715 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4715
//
// <-- Short Description -->
// CLR and DLR blocks ignore context set by scicos_simulate
//

importXcosDiagram(SCI+"/modules/xcos/tests/nonreg_tests/bug_4715.zcos");
%scicos_context.a = 0.1;
%scicos_context.b = 0.1;
global AA
Info = scicos_simulate(scs_m,list(),%scicos_context,"nw");
res = AA;

assert_checkalmostequal(res, 1.56016557141035550060, 1E-4);
