// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 12080 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12080
//
// <-- Short Description -->
// lsqrsolve always print messages

clear;
N=10;
a=2;
b=1;
c=1;
d=1;
x=linspace(-2,4,N)';
noise=0.1*grand(N,1,"nor",0,1);
data=a./(b+c.*(x-d).^2)+noise;
function y=model(p,m,x)
    a=p(1);
    b=p(2);
    c=p(3);
    d=p(4);
    y=a./(b+c.*(x-d).^2);
endfunction

function y=modelDiff(p,m,x,data)
    y=model(p,m,x)-data;
endfunction
m = size(data,"*");
p=[1,1,1,1];
y=model(p,m,x);
y=modelDiff(p,m,x,data);
p0=[1 1 1 1];
lsqrsolve(p0,list(modelDiff,x),m); // No warning must be displayed

clear;
N=10;
a=1;
b=1;
c=1;
d=100000;
x=linspace(-2,4,N)';
noise=0.1*grand(N,1,"nor",0,1);
data=a./(b+c.*(x-d).^2)+noise;

function y=model(p,m,x)
    a=p(1);
    b=p(2);
    c=p(3);
    d=p(4);
    y=a./(b+c.*(x-d).^-10);
endfunction

function y=modelDiff(p,m,x,data)
    y=model(p,m,x)-data;
endfunction
m = size(data,"*");
p=[1,1,1,1];
y=model(p,m,x);
y=modelDiff(p,m,x,data);
p0=[1 1 1 1];
[popt,diffopt]=lsqrsolve(p0,list(modelDiff,x),m,[1d-8,1d-8,1d-5,2,0,100]); // A warning must be displayed
