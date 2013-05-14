// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9844 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9844
//
// <-- Short Description -->
// sum returns wrong values

M=100;N=100;
R=ones(M,N);
for m=1:M;
  for n=1:N;
    A=floor(256*rand(n,m));
    B=uint8(A);
    R(m,n)=max(abs(sum(B,2,"double")-sum(A,2)));
  end
end
F = find(R<>0)
assert_checktrue(isempty(F));
