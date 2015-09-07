//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdfchi() function
// =============================================================================

// Run with test_run('statistics','cdfchi',['no_check_error_output']);

prec = 1.e-5;

x  = [0.01,0.025,0.05,0.1,0.9,0.95];
deff('[y]=chi1(x)','y=exp(-x/2)/sqrt(2*%pi*x)');
y1 = [];

for xx=x
	y1=[y1,intg(0,xx,chi1)];
end

y2 = cdfchi("PQ",x,ones(x));
if norm(y1-y2) > prec then pause,end

df       = [1,2,3,4,5,6];
[P,Q]    = cdfchi("PQ",x,df);
[x1]     = cdfchi("X",df,P,Q);
[df1]    = cdfchi("Df",P,Q,x);

if norm(x1-x)   > prec then pause,end
if norm(df1-df) > prec then pause,end

// IEEE support
// See http://bugzilla.scilab.org/show_bug.cgi?id=7296
Df       = 1;

X        = %inf; // Inf
[P,Q]    = cdfchi("PQ", X, Df);
assert_checkequal(P, 1);
assert_checkequal(Q, 0);

X        = %nan; // NaN
[P,Q]    = cdfchi("PQ", X, Df);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);
