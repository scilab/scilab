//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
a=0.9;b=1;c=1;
sl=syslin('d',a,b,c);
h=ss2tf(sl);
y=flts([1 0 0 0 ],sl);
if norm(y-[0 a.^(0:2)])>1d-15 then pause,end

[y,x]=flts([1 0 0 0 ],sl);
if norm(y-[0 a.^(0:2)])>1d-15 then pause,end
if abs(x-a^3)>1d-15 then pause,end


y=flts([1 0 0 0 ],sl,1);
if norm(y-([0 a.^(0:2)]+a.^(0:3)))>1d-15 then pause,end
[y,x]=flts([1 0 0 0 ],sl,1);
if norm(y-([0 a.^(0:2)]+a.^(0:3)))>1d-15 then pause,end
if abs(x-(a^4+a^3))>1d-15 then pause,end

y=flts([1 0 0 0 ],h);
if norm(y-[0 a.^(0:2)])>1d-15 then pause,end
if execstr("[y,x]=flts([1 0 0 0 ],h);","errcatch")==0 then pause,end
