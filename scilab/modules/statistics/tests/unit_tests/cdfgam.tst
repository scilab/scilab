// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfgam() function
// =============================================================================

prec  = 1.e-5;

shape = 2;
scale = 3;
bn    = 1;

deff('[y]=Gamma(x)','y=bn*(x^(shape-1) * exp(-scale*x))');

bn     = intg(0,10^3,Gamma);
bn     = 1/bn;
x      = 1:10;
[P,Q]  = cdfgam("PQ",x,shape*ones(x),scale*ones(x));
P1     = [];

for xx=x
	P1=[P1,intg(0,xx,Gamma)];
end

if norm(P1-P) > prec then pause,end

shape    = 2*x;
scale    = 3*x;
[P,Q]    = cdfgam("PQ",x,shape,scale);
[x1]     = cdfgam("X",shape,scale,P,Q);
[shape1] = cdfgam("Shape",scale,P,Q,x);
[scale1] = cdfgam("Scale",P,Q,x,shape);

if norm(shape1-shape) > 10*prec then pause,end
if norm(scale1-scale) > 10*prec then pause,end
if norm(x1-x)         > prec    then pause,end
