// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 11105 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11105
//
// <-- Short Description -->
// Crash with color_flag 3

ff=0.1:0.1:0.6;
pp=0:%pi/12:2*%pi;
R=100;

[fd,pd]=meshgrid(ff,pp);
ad=R*sin(fd);
xd=ad .* cos(pd);
yd=ad .* sin(pd);
zd=R*cos(fd);
surf(xd,yd,zd);
h=gce();

assert_checkequal(execstr("h.color_flag=3;","errcatch"), 0);

