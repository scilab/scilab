% Test file for function findstr()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = findstr(['str1'],'s');
% TEST 2
res2 = findstr(['str1 str2'],'s');
% TEST 3
res3 = findstr(['str1','str8'],'s');
% TEST 4
res4 = findstr(['123'],'s');
% TEST 5
res5 = findstr(['str1'],'str14');
% TEST 6
res6 = findstr(['str1 str2'],'str14');
% TEST 7
res7 = findstr(['str1','str8'],'str14');
% TEST 8
res8 = findstr(['123'],'str14');
% TEST 9
res9 = findstr(['str1'],['8']);
% TEST 10
res10 = findstr(['str1 str2'],['8']);
% TEST 11
res11 = findstr(['str1','str8'],['8']);
% TEST 12
res12 = findstr(['123'],['8']);
% TEST 13
res13 = findstr(['str1'],'12');
% TEST 14
res14 = findstr(['str1 str2'],'12');
% TEST 15
res15 = findstr(['str1','str8'],'12');
% TEST 16
res16 = findstr(['123'],'12');
% TEST 17
res17 = findstr(['str1'],['str2','str1']);
% TEST 18
res18 = findstr(['str1 str2'],['str2','str1']);
% TEST 19
res19 = findstr(['str1','str8'],['str2','str1']);
% TEST 20
res20 = findstr(['123'],['str2','str1']);
