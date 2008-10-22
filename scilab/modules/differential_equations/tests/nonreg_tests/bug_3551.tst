// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3551 -->
//
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3551
//
// <-- Short Description -->
// scripts using bvodeS and bvode hang and close Scilab 5.0.1
// same scripts run fine in Scilab 4.1.2

// <-- INTERACTIVE TEST -->

function RhS=fsub(x,z)
  RhS=[(1-6*x*x*z(4)-6*x*z(3))/(z(5)^3);1]
endfunction

function g=gsub(i,z)
  g=[z(1) z(3) z(1)-1 z(3) z(5)-2]
  g=g(i)
endfunction

function [z,lhS]=ystart(x)
  z=zeros(5,1);z(5)=1;
  lhS=[0;1];
endfunction

n=2;
m=[4 1];
N=100;
a=1; b=2;
zeta=[a a b b b];
x=linspace(a,b,N);

ltol=4; // We want to change the default error for y1'''.
tol=1e-12;
z=bvodeS(x,m,n,a,b,fsub,gsub,zeta,ltol=ltol,tol=tol,ystart=ystart);

clear fsub;
clear gsub;

function z=yex(x) // True solution
  z=zeros(5,1);
  z(1)=0.25*(10*log(2)-3)*(1-x)+0.5*(1/x+(3+x)*log(x)-x)+(x-1)
  z(2)=-0.25*(10*log(2)-3)+0.5*(-1/x^2+(3+x)/x+log(x)-1)+1
  z(3)=0.5*(2/x^3+1/x-3/x^2)
  z(4)=0.5*(-6/x^4-1/x/x+6/x^3)
  z(5)=x
endfunction

function rhs=fsub(x,z)
  rhs=[-z(3)*z(1);0]
endfunction

function g=gsub(i,z)
  g=[z(1)-z(2) z(1)-1 z(1)]
  g=g(i)
endfunction

clear ystart;

// The following start function is good for the first 8 eigenfunctions.
function [z,lhs]=ystart(x,z,la0)
  z=[1;0;la0]
  lhs=[0;0]
endfunction

a=0;b=1;
m=[2;1];
n=2;
zeta=[a a b];
N=101;
x=linspace(a,b,N)';

// We have s(n)-(n+1/2)*pi -> 0 for n to infinity.
la0=input('n-th eigenvalue: n= ?');la0=(%pi/2+la0*%pi)^2;

z=bvodeS(x,m,n,a,b,fsub,gsub,zeta,ystart=list(ystart,la0));

a=0;b=1;m=2;n=1;
zeta=[a b];
N=101;
tol=1e-8*[1 1];
x=linspace(a,b,N);

clear fsub
function rhs=fsub(x,z),rhs=-exp(z(1));endfunction

clear gsub
function g=gsub(i,z)
  g=[z(1) z(1)]
  g=g(i)
endfunction

clear ystart
function [z,lhs]=ystart(x,z,M) 
  //z=[4*x*(1-x)*M ; 4*(1-2*x)*M]
  z=[M;0]
  //lhs=[-exp(4*x*(1-x)*M)]
  lhs=0
endfunction

for M=[1 4]
   if M==1
      z=bvodeS(x,m,n,a,b,fsub,gsub,zeta,ystart=list(ystart,M),tol=tol);
   else
      z1=bvodeS(x,m,n,a,b,fsub,gsub,zeta,ystart=list(ystart,M),tol=tol);
   end
end

// Integrating the ode yield e.g. the two solutions yex and yex1. 
clear yex

function y=f(c),y=c.*(1-tanh(sqrt(c)/4).^2)-2;endfunction 
c=fsolve(2,f);

function y=yex(x,c)
  y=log(c/2*(1-tanh(sqrt(c)*(1/4-x/2)).^2))
endfunction 

function y=f1(c1), y=2*c1^2+tanh(1/4/c1)^2-1;endfunction
c1=fsolve(0.1,f1);

function y=yex1(x,c1)
  y=log((1-tanh((2*x-1)/4/c1).^2)/2/c1/c1)
endfunction 

clear fsub;
clear gsub;
a=-1;b=1;c=0;
// The side condition point c must be included in the array fixpnt.
n=1;
m=[3];

function rhs=fsub(x,z)
  rhs=1
endfunction

function g=gsub(i,z)
  g=[z(1)-2 z(1)-1 z(1)-2]
  g=g(i)
endfunction

N=10;
zeta=[a c b];
x=linspace(a,b,N);

z=bvodeS(x,m,n,a,b,fsub,gsub,zeta,fixpnt=c);
  

