// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 954 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=954
//
// <-- Short Description -->
// Scilab is dead after running this script. It is the graphical
// commands at the end. Some test must be missing for
// huge number of points.

// Test #1
function Xd=f(t,X)
  p=X(1);pi=X(2);xh=X(3);z=X(4);
  pd=-(p^2+2*p+1/lam);
  pid=1+2*(1/(lam*p)+1)*pi;
  xhd=-(1+p)*xh;
  zd=-1/(lam*p)*(z-xh)-(1+(1/pi))*z;
  sigd=z^2;
  Xd=[pd;pid;xhd;zd;sigd];
endfunction
  
stacksize(100000000);
lam=.8;

p0=1;
pi0=inv(p0);
xh0=2;
z0=xh0;
sig0=0;
TT=0:.0000001:1;
XX=ode([p0;pi0;xh0;z0;sig0],0,TT,f);

TT=TT(1:size(XX,2));

K=XX($,:)+XX(2,:).*(XX(3,:)-XX(4,:)).^2;
xset('window',0);
clf();
plot2d(TT',K');
xset('window',1);
clf();
plot2d(TT',XX(2,:)');
xset('window',2);
clf();
plot2d(TT',XX(1,:)');

// Test #2
clear
scf();
N=5d6;
stacksize(6*N);
TT=linspace(0,1,N);
plot2d(TT,sin(TT));

