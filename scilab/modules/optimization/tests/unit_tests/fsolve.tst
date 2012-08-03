// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a=[1,7;2,8];b=[10;11];
deff('[y]=fsol1(x)','y=a*x+b');
deff('[y]=fsolj1(x)','y=a');
[xres]=fsolve([100;100],fsol1);
res1 = a*xres+b;
if res1 <> [0;0] then pause,end
[xres]=fsolve([100;100],fsol1,fsolj1);
res2 = a*xres+b;
if res2 <> [0;0] then pause,end
[xres]=fsolve([100;100],'fsol1','fsolj1',1.e-7);
res3 = a*xres+b;
if res3 <> [0;0] then pause,end
