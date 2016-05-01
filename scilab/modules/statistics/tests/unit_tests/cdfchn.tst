//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfchn function
// =============================================================================

// Run with test_run('statistics','cdfchn',['no_check_error_output']);

prec = 1.e-5;

x      = [0.01,0.025,0.05,0.1,0.9,0.95];
y1     = cdfchi("PQ",x,ones(x));
y2     = cdfchn("PQ",x,ones(x),0*ones(x));
if norm(y1-y2) > prec then pause,end

df     = [1,2,3,4,5,6];
pno    = df/10;
[P,Q]  = cdfchn("PQ",x,df,pno);
[x1]   = cdfchn("X",df,pno,P,Q);
[df1]  = cdfchn("Df",pno,P,Q,x);

if norm(x1-x)   > prec then pause,end
if norm(df1-df) > prec then pause,end

// IEEE support
// See http://bugzilla.scilab.org/show_bug.cgi?id=7296
Df       = 1;
Pnonc    = 0.1;

X        = %inf; // Inf
[P,Q]    = cdfchn("PQ", X, Df, Pnonc);
assert_checkequal(P, 1);
assert_checkequal(Q, 0);

X        = %nan; // NaN
[P,Q]    = cdfchn("PQ", X, Df, Pnonc);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);
