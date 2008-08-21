//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// Simple example

my_handle = scf(100001);
clf(my_handle,"reset");
demo_viewCode("ode_simple.dem.sce");

// ODE definition
deff("[xd]=lin(t,x,a)","xd=a*x")
a=[1 1;0 2];

// Integration and comparison with exp
ea = ode(eye(2,2),0,1,list(lin,a)),expm(a);

// New integration with another a
t  = 0:0.1:3;
ee = ode(1,0,t,list(lin,1));

// Visualization
plot2d(t',ee',(1:2),leg="x1@x2");
a = gca();
a.title.text="Simple ODE";
a.title.font_size=3;

clear lin a t ee ea
