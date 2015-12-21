// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy <jean-baptiste.silvy@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 2711 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2708
//
// <-- Short Description -->
// The plot obtained with "champ" function is bad



function yprim=f(t,y); yprim=y^2-t; endfunction
t=-3:5; y=-3:3; nt=length(t); ny=length(y);
ft=ones(nt,ny); fy=feval(t,y,f);
champ(t,y,ft,fy);

a = gca();

// champ data_bounds must be outside [-3,5] x [-3,3]
if (a.data_bounds(1,1) >= -2.99) then, pause;end
if (a.data_bounds(2,1) <= 5) then, pause;end
if (a.data_bounds(1,2) >= -2.99) then, pause;end
if (a.data_bounds(2,2) <= 3) then, pause;end

