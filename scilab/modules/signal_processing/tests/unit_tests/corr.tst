// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


rand('normal');
x=rand(1,256);
y=-x;
deff('[z]=xx(inc,is)','z=x(is:is+inc-1)');
deff('[z]=yy(inc,is)','z=y(is:is+inc-1)');
[c, mxy]=corr(x,y,32);
if or(size(c)<>[1 32]) then pause,end
if or(size(mxy)<>[1 2]) then pause,end

x=x-mxy(1)*ones(x);
y=y-mxy(2)*ones(y);
c1=corr(x,y,32);
c2=corr(x,32);
if norm(c1+c2,1) > %eps then pause,end

[c3,m3]=corr('fft',xx,yy,256,32);
if norm(c1-c3,1) > 10 *%eps then pause,end

[c4,m4]=corr('fft',xx,256,32);
if or(size(c4)<>[1 32]) then pause,end
if or(size(m4)<>[1 1]) then pause,end

if norm(m3,1) > %eps then pause,end
if norm(m4,1) > %eps then pause,end
if norm(c3-c1,1) > 10*%eps then pause,end
if norm(c4-c2,1) > 10*%eps then pause,end

 