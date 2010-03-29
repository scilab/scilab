% Test file for function expm()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = expm([]);
% TEST 2
res2 = expm(m2sciUnknownType([]));
% TEST 3
res3 = expm(m2sciUnknownDims([]));
% TEST 4
res4 = expm([1]);
% TEST 5
res5 = expm([1,2,3;4,5,6;7,8,0]);
% TEST 6
res6 = expm(m2sciUnknownType([1]));
% TEST 7
res7 = expm(m2sciUnknownType([1,2,3;4,5,6;7,8,0]));
% TEST 8
res8 = expm(m2sciUnknownDims([1]));
% TEST 9
res9 = expm(m2sciUnknownDims([1,2,3;4,5,6;7,8,0]));
% TEST 10
res10 = expm([i]);
% TEST 11
res11 = expm([i,2i,3i;4i,5i,6i;7i,8i,0]);
% TEST 12
res12 = expm(m2sciUnknownType([i]));
% TEST 13
res13 = expm(m2sciUnknownType([i,2i,3i;4i,5i,6i;7i,8i,0]));
% TEST 14
res14 = expm(m2sciUnknownDims([i]));
% TEST 15
res15 = expm(m2sciUnknownDims([i,2i,3i;4i,5i,6i;7i,8i,0]));
