// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6477 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6477
//
// <-- Short Description -->
// in some case, schur returned wrong values on Windows.

function [f] = z_choose(s,t)
  f = abs(s) > abs(t)
endfunction

a=[1 4 5 6; 3 2 5 7; 8 3 4 5; 9 3 5 2];
b=[3 8 5 7; 1 4 9 3; 9 1 0 7; 9 2 4 8];
[as, bs, z, dim] = schur(a, b, z_choose);

if dim <> 2 then pause, end

ref_as = [ - 5.8165015,    4.9422182,  - 1.6845626,    1.8997122  ;..
    0.,           9.4018462,  - 14.867682,  - 2.1905808  ;..
    0.,           0.,           3.4464284,    2.2480735  ;..
    0.,           0.,         - 1.0118069,    1.0484945];

ref_bs = [0.5173089,    6.2627142,  - 2.9573865,    7.2946208  ;..
    0.,           8.7992813,  - 16.353371,  - 2.5799548  ;..
    0.,           0.,           8.0516398,    0.           ;..
    0.,           0.,           0.,           5.6752093  ];

ref_z = [0.4973431,  - 0.6831312,    0.2943626,  - 0.4464664; ..
    0.4702536,    0.5375540,    0.6832618,    0.1518237; ..
    0.0048984,  - 0.4789154,    0.1826199,    0.8586419; ..
  - 0.7290318,  - 0.1225046,    0.6427705,  - 0.2008765];
  
if or(abs(as - ref_as) > 1e10 * %eps) then pause, end
if or(abs(bs - ref_bs) > 1e10 * %eps) then pause, end
if or(abs(z - ref_z) > 1e9 * %eps) then pause, end
