// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
a=-1/3;b=1;c=1;
sl=syslin('c',a,b,c);
sld=dscr(sl,0.1);
if sld.dt<>0.1 then pause,end
if sld.A<>exp(sl.A*0.1) then pause,end
if norm(sld.B-sl.A\(sld.A-eye())*sl.B)>1d-15 then pause,end


a=[0.9,0,0.6,-1.4,-4.2;
   0.2,0.1,-0.2,0.5,0.6;
   -4.3,0,2.2,0,2.4;
   -3.7,-0.5,2.4,-0.6,2.7;
   6.4,0.1,-4,-0.5,-4];

b=[-0.1,-0.1,0;
   0,0,0.1;
   -0.1,0.2,-0.1;
   0.2,0.2,-0.6;
   0.2,-0.1,0.1];
 
c=[2,7,-2,5,1
   0,-1,3,0,2];
d=[1,0,0
   0,0,0];

sl=syslin('c',a,b,c,d);

sld=dscr(sl,0.1);
if sld.dt<>0.1 then pause,end
if norm(sld.A-expm(sl.A*0.1))>1d-15 then pause,end
if norm(sld.B-sl.A\(sld.A-eye())*sl.B)>1d-14 then pause,end

