% Test file for function zeros()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = zeros(2);
% TEST 2
res2 = zeros(3,5);
% TEST 3
res3 = zeros(4,4);
% TEST 4
res4 = zeros([10,10]);
% TEST 5
res5 = zeros(size([1 2]));
% TEST 6
res6 = zeros(m2sciUnknownDims([1,2]));
