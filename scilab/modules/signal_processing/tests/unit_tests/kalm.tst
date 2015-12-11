//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte Hecquet
// Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

w=%pi/4; // angular frequency
T=0.1; // period
t=0:T:5;
signal=cos(w*t);
// Sinusoid with noise
v=0:1:50;
y=signal+v;
// System
n=2; // system order
f=[cos(w*T) -sin(w*T); sin(w*T) cos(w*T)];
g=0;
h=[1 0];
p0=[1000 0; 0 0];
R=1;
Q=0;
x0=zeros(n,1);
// Initialize for loop
x1=x0;
p1=p0;
// Kalman filter
xsum=0;
x1sum=0;
psum=0;
p1sum=0;
for i=1:length(t)-1
    [x1(:,i+1),p1,x,p]=kalm(y(i),x1(:,i),p1,f,g,h,Q,R);
    xsum = xsum + x;
    p1sum = p1sum + p1;
    psum = psum + p;
    x1sum = x1sum + x1(:,i+1);
end
assert_checkalmostequal(xsum, [295.374859628719719;202.134913816696837]);

assert_checkalmostequal(p1sum,  [3.88430845743643571,0.63055345519216977;0.63055345519217121,2.08824467576958606]);

assert_checkalmostequal(psum, [3.97189248710404863,0.48230715738369057;0.48230715738369184,2.0006606461019758]);
assert_checkalmostequal(x1sum,  [278.60499495977632;224.686643723725581]);

