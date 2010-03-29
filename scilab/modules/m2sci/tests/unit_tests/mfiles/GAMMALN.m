% Test file for function gammaln()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = gammaln([]);
% TEST 2
res2 = gammaln(m2sciUnknownType([]));
% TEST 3
res3 = gammaln(m2sciUnknownDims([]));
% TEST 4
res4 = gammaln([1]);
% TEST 5
res5 = gammaln([1,2,3]);
% TEST 6
res6 = gammaln([1;2;3]);
% TEST 7
res7 = gammaln([1,2,3;4,5,6]);
% TEST 8
res8 = gammaln(m2sciUnknownType([1]));
% TEST 9
res9 = gammaln(m2sciUnknownType([1,2,3]));
% TEST 10
res10 = gammaln(m2sciUnknownType([1;2;3]));
% TEST 11
res11 = gammaln(m2sciUnknownType([1,2,3;4,5,6]));
% TEST 12
res12 = gammaln(m2sciUnknownDims([1]));
% TEST 13
res13 = gammaln(m2sciUnknownDims([1,2,3]));
% TEST 14
res14 = gammaln(m2sciUnknownDims([1;2;3]));
% TEST 15
res15 = gammaln(m2sciUnknownDims([1,2,3;4,5,6]));
