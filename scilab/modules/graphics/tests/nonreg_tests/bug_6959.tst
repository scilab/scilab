// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2010 - DIGITEO - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 6959 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6959
//
// <-- Short Description -->
// axis.tics_labels does not accept $..$ LaTeXed labels

clf
plot2d(0:1,0:1,rect=[0 0 1 1],nax=[1 6 1 6])
a=gca();
p=a.x_ticks.locations;
v0=a.x_ticks.labels;
drawaxis(x=a.x_ticks.locations,y=0.5,dir="u",tics="v",val=v0);
e=gce();
v1="$"+v0+"$";
e.tics_labels=v1';

