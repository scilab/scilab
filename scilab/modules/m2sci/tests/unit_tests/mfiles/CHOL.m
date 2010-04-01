% Test file for function chol()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = chol([]);
% TEST 2
res2 = chol(m2sciUnknownType([]));
% TEST 3
res3 = chol(m2sciUnknownDims([]));
% TEST 4
res4 = chol(1);
% TEST 5
res5 = chol(eye(3,3));
