// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfnor() function
// =============================================================================

prec  = 1.e-5;

x     = 0:0.1:3;
[p,q] = cdfnor("PQ",x,0.0*ones(x),1.0*ones(x));
if norm( p - 1/2*(1+erf(x/sqrt(2))))> 100*%eps then pause,end
yy    = cdfnor("PQ",0.5,0.0,1.0);

deff('y=f(t)','y=exp(-t^2/2)');
if norm(yy-(1/2+ 1/sqrt(2*%pi)*intg(0,0.5,f)))> %eps then pause,end

[x1] = cdfnor("X",0.0*ones(x),1.0*ones(x),p,q);
if norm(x-x1) > prec then pause,end 

M     = 2*x;
[p,q] = cdfnor("PQ",x,M,1.0*ones(x));
M1    = cdfnor("Mean",1.0*ones(x),p,q,x);
if norm(M-M1) > prec then pause,end

// avoid M=x
M      = 2*x+1;
Std    = x+1;
[p,q]  = cdfnor("PQ",x,M,Std);
Std1   = cdfnor("Std",p,q,x,M);
if norm(Std-Std1) > prec then pause,end
