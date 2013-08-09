// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 2682-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2682
//
// <-- Short Description -->
// chart() is inconsistent in the second argument, angle. It might be treated as degrees or radian.
// 

s=poly(0,'s');
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));


f0 = scf(0);
black(h,0.01,100,'(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)');
chart([-8 -6 -4],[80 120]);
c0 = gce();

f1 = scf(1);
black(h,0.01,100,'(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01)');
chart([-8 -6 -4],[80 120],list(0,0));
c1 = gce();

// The two above figure should be identical
// the current entities are compounds containing two polylines
if (c0.children(1).data <> c1.children(1).data) then pause; end
if (c0.children(2).data <> c1.children(2).data) then pause; end
