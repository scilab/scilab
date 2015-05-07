// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte Denizet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Unit test for coloured marks -->
//
// <-- Short Description -->
// Tests the coloured mark on a 3d plot
//
// <-- INTERACTIVE TEST -->


function newH = fall(h, v, t)
    i = find(h ~= 0);
    newH(i) = h(i)+v(i)*t-0.5*9.81*t^2
    j = find(newH < 0);
    newH(j) = 0;
endfunction

drawlater();
clf();
plot3d();
a=gca();
p=a.children(1);
p.mark_background=-3;
p.mark_size=1;
p.mark_mode="on";
p.surface_mode="off";
p.data.z = p.data.z + 10; 
a.data_bounds=[-%pi -%pi 9; %pi %pi 11];
drawnow();

t=0;
step = 0.01;
hinit = p.data.z;
h = hinit;
v=4*(rand(hinit) - 0.5);
while or(h)
    t = t + step;
    h = fall(hinit, v, t);
    p.data.z = h;
    sleep(20);
end
