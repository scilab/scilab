// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6829 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6829
//
// <-- Short Description -->
// The cacsd module graphic function (bode, black, nyquist,...) do not preserve the immediate_drawing property value.

fig=gcf();
s=poly(0,'s');
h=syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01));


clf();fig.immediate_drawing="off";
bode(h);
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
gainplot(h);
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
show_margins(h);
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
black(h);
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
nyquist(h);
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
hallchart();
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
nicholschart();
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
sgrid();
if fig.immediate_drawing=="on" then pause,end

clf();fig.immediate_drawing="off";
zgrid();
if fig.immediate_drawing=="on" then pause,end

