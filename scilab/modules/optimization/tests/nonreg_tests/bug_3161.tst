// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3161 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3161
//
// <-- Short Description -->
// datafit crashes

function y=FF(x,p),y=p(1)*(x-p(2))+p(3)*x.*x,endfunction

X=[];Y=[];

pg=[34;12;14]; //parameter used to generate data
for x=0:.1:3, Y=[Y,FF(x,pg)+100*(rand()-.5)];X=[X,x];end

Z=[Y;X];

//The criterion function
function e=G(p,z)
   y=z(1),x=z(2);
   e=y-FF(x,p);
endfunction

//Solve the problem
p0=[3;5;10];

p = [];
err = [];
[p,err]=datafit(G,Z,p0);

if p == [] then pause,end;
if err == [] then pause,end;