// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre Lando
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6686 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6686
//
// <-- Short Description -->
// The automatic sub-ticking gave wrong result.
// 
// Launch the script below and lock at the correct sub-ticking.


x = logspace(-4,4,1000);
y = 1 ./ x;

scf();
plot(x,y)
a = gca();

a.log_flags = 'lln';

saved_db = a.data_bounds;
a.data_bounds = [0.001, 0.001; 100, 100 ];
a.data_bounds = [0.001, 0.001; 1000, 1000 ];
a.data_bounds = saved_db;

a.log_flags = 'nnn';
a.data_bounds = [0.001, 0.001; 10, 10 ];

for inc = 1.2:0.1:2.0
  xt = [0:inc:10]; a.x_ticks = tlist('ticks', xt, string(xt));
  halt('press enter to continue');
end

// Next try zooming on the plot
a.auto_ticks = ['on', 'on', 'on'];
