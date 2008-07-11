// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
s=%s;
n=[1+s   2+3*s+4*s^2        5; 0        1-s             s];
d=[1+3*s   5-s^3           s+1;1+s     1+s+s^2      3*s-1];

h=syslin('c',n./d);
[n,d]=simp(n,d);
if h<>rlist(n,d,'c') then pause,end
sl=tf2ss(h); 
e=h-ss2tf(sl);
if norm(coeff(e(2)))>100000*%eps then pause,end
 
