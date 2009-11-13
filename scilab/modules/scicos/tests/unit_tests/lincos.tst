// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
ilib_verbose(0);

exec("SCI/modules/xcos/demos/PENDULUM_ANIM.sci");
exec("SCI/modules/xcos/demos/anim_pen.sci");
load("SCI/modules/scicos/tests/unit_tests/pendulum_anim45.cos");
M=10;
m=3;
l=3;
ph=0.1; 
scs_m = scs_m.objs(5).model.rpar;
[X,U,Y,XP] = steadycos(scs_m,[],[],[],[],1,1:$);
sys = lincos(scs_m,X,U);

