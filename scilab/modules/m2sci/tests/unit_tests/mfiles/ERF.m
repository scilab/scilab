% Test file for function erf()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = erf([]);
% TEST 2
res2 = erf(m2sciUnknownType([]));
% TEST 3
res3 = erf(m2sciUnknownDims([]));
% TEST 4
res4 = erf([1]);
% TEST 5
res5 = erf([1,2,3]);
% TEST 6
res6 = erf([1;2;3]);
% TEST 7
res7 = erf([1,2,3;4,5,6]);
% TEST 8
res8 = erf(m2sciUnknownType([1]));
% TEST 9
res9 = erf(m2sciUnknownType([1,2,3]));
% TEST 10
res10 = erf(m2sciUnknownType([1;2;3]));
% TEST 11
res11 = erf(m2sciUnknownType([1,2,3;4,5,6]));
% TEST 12
res12 = erf(m2sciUnknownDims([1]));
% TEST 13
res13 = erf(m2sciUnknownDims([1,2,3]));
% TEST 14
res14 = erf(m2sciUnknownDims([1;2;3]));
% TEST 15
res15 = erf(m2sciUnknownDims([1,2,3;4,5,6]));
