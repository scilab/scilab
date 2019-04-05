// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 16020 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16020
//
// <-- Short Description -->
// cov() was slow
// =============================================================================
// legacy code
n=20;
x=rand(1e6,n);
for i = 1:n
    x(:,i) = x(:,i) - mean(x(:,i));
end
tic
c = zeros(n, n);
for i = 1:n
    c(i,i) = x(:,i)'*x(:,i);
    for j = i+1:n
        c(i,j) = x(:,i)'*x(:,j);
        c(j,i) = c(i,j);
    end
end
t0 = toc(); // typically t0 = 5 seconds

// large memory bandwidth implementation
x=rand(1e6,n);
tic;
x = center(x,"r");
c = x'*x;
t1 = toc(); // typically t0 = 1 seconds

// new implementation (c++ gateway)
x=rand(1e6,n);
tic
c = cov(x);
t2=toc(); // typically t2 = 0.06 seconds

assert_checktrue(t1/t2 > 4);
assert_checktrue(t0/t2 > 30);