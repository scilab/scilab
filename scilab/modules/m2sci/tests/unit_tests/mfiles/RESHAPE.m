% Test file for function reshape()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = reshape('abc',3,[]);
% TEST 2
res2 = reshape('abc',3,1);
% TEST 3
res3 = reshape([1,2,3;4,5,6],[1,6]);
% TEST 4
res4 = reshape([1,2,3]==[1,0,3],1,3);
