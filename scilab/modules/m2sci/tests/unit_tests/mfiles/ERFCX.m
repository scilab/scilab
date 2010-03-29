% Test file for function erfcx()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = erfcx([]);
% TEST 2
res2 = erfcx(m2sciUnknownType([]));
% TEST 3
res3 = erfcx(m2sciUnknownDims([]));
% TEST 4
res4 = erfcx([1]);
% TEST 5
res5 = erfcx([1,2,3]);
% TEST 6
res6 = erfcx([1;2;3]);
% TEST 7
res7 = erfcx([1,2,3;4,5,6]);
% TEST 8
res8 = erfcx(m2sciUnknownType([1]));
% TEST 9
res9 = erfcx(m2sciUnknownType([1,2,3]));
% TEST 10
res10 = erfcx(m2sciUnknownType([1;2;3]));
% TEST 11
res11 = erfcx(m2sciUnknownType([1,2,3;4,5,6]));
% TEST 12
res12 = erfcx(m2sciUnknownDims([1]));
% TEST 13
res13 = erfcx(m2sciUnknownDims([1,2,3]));
% TEST 14
res14 = erfcx(m2sciUnknownDims([1;2;3]));
% TEST 15
res15 = erfcx(m2sciUnknownDims([1,2,3;4,5,6]));
