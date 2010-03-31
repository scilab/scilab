% Test file for function erfc()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = erfc([]);
% TEST 2
res2 = erfc(m2sciUnknownType([]));
% TEST 3
res3 = erfc(m2sciUnknownDims([]));
% TEST 4
res4 = erfc([1]);
% TEST 5
res5 = erfc([1,2,3]);
% TEST 6
res6 = erfc([1;2;3]);
% TEST 7
res7 = erfc([1,2,3;4,5,6]);
% TEST 8
res8 = erfc(m2sciUnknownType([1]));
% TEST 9
res9 = erfc(m2sciUnknownType([1,2,3]));
% TEST 10
res10 = erfc(m2sciUnknownType([1;2;3]));
% TEST 11
res11 = erfc(m2sciUnknownType([1,2,3;4,5,6]));
% TEST 12
res12 = erfc(m2sciUnknownDims([1]));
% TEST 13
res13 = erfc(m2sciUnknownDims([1,2,3]));
% TEST 14
res14 = erfc(m2sciUnknownDims([1;2;3]));
% TEST 15
res15 = erfc(m2sciUnknownDims([1,2,3;4,5,6]));
