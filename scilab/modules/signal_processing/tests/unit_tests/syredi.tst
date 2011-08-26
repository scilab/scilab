// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


[fact,b2,b1,b0,c1,c0,zzeros,zpoles]=syredi(1,4,[2*%pi/10,4*%pi/10,0,0],0.02,0.001);
ref_fact = 0.005976;
if norm(fact-ref_fact) > 0.0000037 then pause,end
if or(b2<>[1 1 1]) then pause,end
if or(size(b1)<>[1 3]) then pause,end
if b1(3)<> 1 then pause,end
if or(b0<>[1 1 0]) then pause,end
if or(size(c1)<>[1 3]) then pause,end
if or(size(c0)<>[1 3]) then pause,end
if c0(3)<> 0 then pause,end
if or(size(zzeros)<>[1 5]) then pause,end
if or(size(zpoles)<>[1 5]) then pause,end
