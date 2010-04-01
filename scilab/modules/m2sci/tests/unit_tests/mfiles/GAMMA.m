% Test file for function gamma()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = gamma([]);
% TEST 2
res2 = gamma(m2sciUnknownType([]));
% TEST 3
res3 = gamma(m2sciUnknownDims([]));
% TEST 4
res4 = gamma([1]);
% TEST 5
res5 = gamma([1,2,3]);
% TEST 6
res6 = gamma([1;2;3]);
% TEST 7
res7 = gamma([1,2,3;4,5,6]);
% TEST 8
res8 = gamma(m2sciUnknownType([1]));
% TEST 9
res9 = gamma(m2sciUnknownType([1,2,3]));
% TEST 10
res10 = gamma(m2sciUnknownType([1;2;3]));
% TEST 11
res11 = gamma(m2sciUnknownType([1,2,3;4,5,6]));
% TEST 12
res12 = gamma(m2sciUnknownDims([1]));
% TEST 13
res13 = gamma(m2sciUnknownDims([1,2,3]));
% TEST 14
res14 = gamma(m2sciUnknownDims([1;2;3]));
% TEST 15
res15 = gamma(m2sciUnknownDims([1,2,3;4,5,6]));
