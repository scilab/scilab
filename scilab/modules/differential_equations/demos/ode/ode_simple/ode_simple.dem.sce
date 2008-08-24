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
function xd=lin(t,x,A),xd=A*x,endfunction
A=[1 1;0 2];

// Integration and comparison with exp
ea = ode(eye(2,2),0,1,list(lin,A)),expm(A);

// New integration with another a
t  = 0:0.1:3;
ee = ode(1,0,t,list(lin,1));

// Visualization
plot2d(t',ee',style=2);
xlabel('t');ylabel('x');
title(['Simple ODE'
       ' xdot=x'],'fontsize',3)
   
