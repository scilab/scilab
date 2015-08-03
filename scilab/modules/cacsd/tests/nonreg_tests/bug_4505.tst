// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4505 -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4505
//
// <-- Short Description -->
//  A red vertical bar appeared  for some bode plots of continuous time systems.


//check there is not Shannon limit curve for continuous systems
sys=syslin("c",1/(1+%s));
clf();bode(sys,0.001,1000)
f=gcf();
a_phase=f.children(1);
a_modulus=f.children(2);
phase_curves=a_phase.children;
if size(phase_curves,"*")<>1 then pause,end
modulus_curves=a_modulus.children;
if size(modulus_curves,"*")<>1 then pause,end

//check there is  Shannon limit curve for discrete systems
sys=dscr(sys,0.01);
clf();bode(sys,0.001,1000)
f=gcf();
a_phase=f.children(1);
a_modulus=f.children(2);
phase_curves=a_phase.children;
if size(phase_curves,"*")<>2 then pause,end
if or(size(phase_curves(1).data)<>[2 2]) then pause,end
modulus_curves=a_modulus.children;
if size(modulus_curves,"*")<>2 then pause,end
if or(size(modulus_curves(1).data)<>[2 2]) then pause,end
