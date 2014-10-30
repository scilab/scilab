// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2003 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2003
//
// <-- Short Description -->
//lusolve crashes scilab when handle has been freed

a=[0.2,0.6,0.6,0.2,0.3;
0.8,0.8,0.7,0.2,0.9;
0,0.7,0.7,0.2,0.2;
0.3,0.9,0.2,0.9,0.3;
0.7,0.1,0.5,0.7,0.4];
b=[0.3;0.6;0.5;0.3;0.6];
A=sparse(a);
[h,rk]=lufact(A);
x=lusolve(h,b);
if norm(a*x-b)>1d-10 then pause,end
[P,L,U,Q]=luget(h);
if norm(P*L*U*Q-A)>1d-10 then pause,end
ludel(h)
if execstr("x=lusolve(h,b);","errcatch")<>999 then pause,end
if execstr("ludel(h);","errcatch")<>999 then pause,end
if execstr("[P,L,U,Q]=luget(h);","errcatch")<>999 then pause,end

//try to allocate a lot of handles
for k=1:20
    [h1,rk]=lufact(A);
end

x=lusolve(h1,b);
if norm(a*x-b)>1d-10 then pause,end
