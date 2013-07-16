// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->


ilib_verbose(0);

prot = funcprot();
funcprot(0);
exec("SCI/modules/xcos/tests/unit_tests/PENDULUM_ANIM.sci");
exec("SCI/modules/xcos/tests/unit_tests/anim_pen.sci");
funcprot(prot);
importXcosDiagram("SCI/modules/xcos/tests/unit_tests/pendulum_anim45.zcos");

M  = 10;
m  = 3;
l  = 3;
ph = 0.1;
z0 = -4;
th0 = .02;

for i=1:length(scs_m.objs)
    if typeof(scs_m.objs(i))=="Block" & scs_m.objs(i).gui=="SUPER_f" then
        scs_m = scs_m.objs(i).model.rpar;
        break;
    end
end

// scs_m is the top SUPER_f;
[X,U,Y,XP] = steadycos(scs_m,[],[],[],[],1,1:$);
sys        = lincos(scs_m,X,U);

// valid results
A_ref = [..
    0.    0.   -8.962D-08   -2.9195682  ;..
    1.    0.    0.           0.         ;..
    0.    0.    2.972D-08    4.2383276  ;..
    0.    0.    1.           0.         ];

B_ref = [..
    0.0997019  ;..
    0.         ;..
   -0.0330679  ;..
    0.         ];

C_ref = [..
    0.    1.    0.    0.  ;..
    0.    0.    0.    1.  ];

D_ref = [..
    0.  ;..
    0.  ];

// diff
margin = 5D-08;
if and(abs(sys.A - A_ref) > margin) |..
   and(abs(sys.B - B_ref) > margin) |..
   and(abs(sys.D - D_ref) > margin) |..
   and(abs(sys.C - C_ref) > margin) then pause, end

