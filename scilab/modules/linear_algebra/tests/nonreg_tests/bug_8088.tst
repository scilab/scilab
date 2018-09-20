// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8088 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8088
//
// <-- Short Description -->
// norm() doesn't handle matrices containing %inf or %nan.

//
// Inf and NaN Exceptions
// By convention, norm(X,p) == Inf if X contains Infs and no NaNs and p != -%inf
// and norm(X,p) == NaN  if X contains NaNs.
//
// vector
//
for p=list(-1,0,1,2,3,%inf,"inf","fro")
  assert_checkequal(norm([1 %nan],p), %nan);
  assert_checkequal(norm([%inf %nan],p), %nan);
  assert_checkequal(norm([%i %nan],p), %nan);
  assert_checkequal(norm([1 %inf],p), %inf);
  assert_checkequal(norm([%i %inf],p), %inf);
end
assert_checkequal(norm([1 %inf],-%inf), 1);
assert_checkequal(norm([%i %inf],-%inf), 1);
assert_checkequal(norm([%nan %inf],-%inf), %nan);
assert_checkequal(norm([%i %inf %nan],-%inf), %nan);
//
// matrix
//
for p=list(1,2,%inf,"inf","fro")
  assert_checkequal(norm([1 0; %nan 0],p), %nan);
  assert_checkequal(norm([1 0; %inf 0],p), %inf);
  assert_checkequal(norm([%i 0; %nan 0],p), %nan);
  assert_checkequal(norm([%i 0; %inf 0],p), %inf);
  assert_checkequal(norm([%inf 0; %nan 0],p), %nan);
end
assert_checkequal(norm([1 0; %nan 0],-%inf), %nan);
assert_checkequal(norm([1 0; %inf 0],-%inf), 1);
assert_checkequal(norm([%i 0; %nan 0],-%inf), %nan);
assert_checkequal(norm([%i 0; %inf 0],-%inf), 1);
assert_checkequal(norm([%inf 0; %nan 0],-%inf), %nan);