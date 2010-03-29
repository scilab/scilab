% Test file for function rcond()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = rcond([]);
% TEST 2
res2 = rcond(m2sciUnknownType([]));
% TEST 3
res3 = rcond(m2sciUnknownDims([]));
% TEST 4
res4 = rcond([1]);
% TEST 5
res5 = rcond([1,2,3;4,5,6;7,8,0]);
% TEST 6
res6 = rcond(m2sciUnknownType([1]));
% TEST 7
res7 = rcond(m2sciUnknownType([1,2,3;4,5,6;7,8,0]));
% TEST 8
res8 = rcond(m2sciUnknownDims([1]));
% TEST 9
res9 = rcond(m2sciUnknownDims([1,2,3;4,5,6;7,8,0]));
% TEST 10
res10 = rcond([i]);
% TEST 11
res11 = rcond([i,2i,3i;4i,5i,6i;7i,8i,0]);
% TEST 12
res12 = rcond(m2sciUnknownType([i]));
% TEST 13
res13 = rcond(m2sciUnknownType([i,2i,3i;4i,5i,6i;7i,8i,0]));
% TEST 14
res14 = rcond(m2sciUnknownDims([i]));
% TEST 15
res15 = rcond(m2sciUnknownDims([i,2i,3i;4i,5i,6i;7i,8i,0]));
