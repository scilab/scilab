% Test file for function linspace()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = linspace(1,10);
% TEST 2
res2 = linspace(1+i,10+10i);
% TEST 3
res3 = linspace(1,'a',1);
% TEST 4
res4 = linspace('a','z',10);
% TEST 5
res5 = linspace(1,1000,'a');
% TEST 6
res6 = linspace(1,m2sciUnknownType('a'),1);
% TEST 7
res7 = linspace(m2sciUnknownType('a'),m2sciUnknownType('z'),10);
% TEST 8
res8 = linspace(1,1000,m2sciUnknownType('a'));
% TEST 9
res9 = linspace(1,1000,[]);
