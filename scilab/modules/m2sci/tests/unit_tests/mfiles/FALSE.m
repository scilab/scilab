% Test file for function false()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = false(2);
% TEST 2
res2 = false(3,5);
% TEST 3
res3 = false(4,4);
% TEST 4
res4 = false([10,10]);
% TEST 5
res5 = false(size([1 2]));
% TEST 6
res6 = false(m2sciUnknownDims([1,2]));
