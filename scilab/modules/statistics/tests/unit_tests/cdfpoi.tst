// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfpoi() function
// =============================================================================

prec  = 1.e-5;

xlam  = 0.34;

deff('k=fact(n)','if n<=1;k=1;else k=n*fact(n-1);end');
deff('s=S(k)','s=0;for j=0:k,s=s+ exp(-xlam)*(xlam)^j/fact(j);end');

X     = 0:10;
C     = feval(X,S);
[P,Q] = cdfpoi("PQ",X,xlam*ones(X));

if norm(P-C) > 100*%eps then pause,end

xlam   = 0.1*(1:11);
X      = 0:10;
[P,Q]  = cdfpoi("PQ",X,xlam);
X1     = cdfpoi("S",xlam,P,Q);
xlam1  = cdfpoi("Xlam",P,Q,X);

if norm(X1-X) > prec then pause,end
if norm(xlam1-xlam) > prec then pause,end
