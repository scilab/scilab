% Test file for function eye()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = eye(2);
% TEST 2
res2 = eye(3,5);
% TEST 3
res3 = eye(4,4);
% TEST 4
res4 = eye([10,10]);
% TEST 5
res5 = eye(size([1 2]));
% TEST 6
res6 = eye(m2sciUnknownDims([1,2]));
