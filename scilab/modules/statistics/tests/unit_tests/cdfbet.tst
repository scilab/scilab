//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for beta distribution
// =============================================================================

// Run with test_run('statistics','cdfbet',['no_check_error_output']);

prec = 1.e-5;

A  = 2;
B  = 3;
bn = 1;

deff('[y]=Beta(x)','y=bn*(x^(A-1) * (1-x)^(B-1))');

bn = intg(0,1,Beta);
bn = 1/bn;
assert_checkequal(intg(0,1,Beta), 1);

x  = 0:0.1:1;
y  = 1-x;
p1 = [];

for k=x
    p1=[p1,intg(0,k,Beta)];
end

A = 2*ones(x);
B = 3*ones(x);

[p,q]=cdfbet('PQ',x,y,A,B);

assert_checkalmostequal(p, p1);

[x1,y1]=cdfbet('XY',A,B,p,q);

assert_checkalmostequal(x, x1);
assert_checkalmostequal(y, y1);

A1 = cdfbet('A',B,p,q,x,y);
assert_checkalmostequal(A1(2:$-1), A(2:$-1));

B1 = cdfbet('B',p,q,x,y,A);
assert_checkalmostequal(B1(2:$-1), B(2:$-1));

// IEEE support
// See http://bugzilla.scilab.org/show_bug.cgi?id=7296
X        = 0;
Y        = 1;
A        = 1;
B        = 1;

X        = %nan; // X NaN
[P,Q]    = cdfbet("PQ", X, Y, A, B);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);

X        = 1;
Y        = %nan; // Y NaN
[P,Q]    = cdfbet("PQ", X, Y, A, B);
assert_checkequal(P, %nan);
assert_checkequal(Q, %nan);
