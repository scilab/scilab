//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//
mode(7)
//To quit this demo, enter CTRL+C, then type "abort"

//                    OPTIMIZATION
deff("[f,g,ind]=rosenbro(x,ind)", "a=x(2)-x(1)^2 , b=1-x(2) ,...
f=100.*a^2 + b^2 , g(1)=-400.*x(1)*a , g(2)=200.*a -2.*b ");
[f,x,g]=optim(rosenbro,[2;2],"qn")

//                   SIMULATION
a=rand(3,3)
e=expm(a)
deff("[ydot]=f(t,y)","ydot=a*y");
e(:,1)-ode([1;0;0],0,1,f)
