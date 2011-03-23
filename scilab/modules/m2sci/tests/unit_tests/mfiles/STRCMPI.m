% Test file for function strcmpi()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = strcmpi(['str1'],'s');
% TEST 2
res2 = strcmpi(['str1 str2'],'s');
% TEST 3
res3 = strcmpi(['str1','str8'],'s');
% TEST 4
res4 = strcmpi(['123'],'s');
% TEST 5
res5 = strcmpi(['idem'],'s');
% TEST 6
res6 = strcmpi(0,'s');
% TEST 7
res7 = strcmpi(1,'s');
% TEST 8
res8 = strcmpi(['str1'],'str14');
% TEST 9
res9 = strcmpi(['str1 str2'],'str14');
% TEST 10
res10 = strcmpi(['str1','str8'],'str14');
% TEST 11
res11 = strcmpi(['123'],'str14');
% TEST 12
res12 = strcmpi(['idem'],'str14');
% TEST 13
res13 = strcmpi(0,'str14');
% TEST 14
res14 = strcmpi(1,'str14');
% TEST 15
res15 = strcmpi(['str1'],['8']);
% TEST 16
res16 = strcmpi(['str1 str2'],['8']);
% TEST 17
res17 = strcmpi(['str1','str8'],['8']);
% TEST 18
res18 = strcmpi(['123'],['8']);
% TEST 19
res19 = strcmpi(['idem'],['8']);
% TEST 20
res20 = strcmpi(0,['8']);
% TEST 21
res21 = strcmpi(1,['8']);
% TEST 22
res22 = strcmpi(['str1'],'12');
% TEST 23
res23 = strcmpi(['str1 str2'],'12');
% TEST 24
res24 = strcmpi(['str1','str8'],'12');
% TEST 25
res25 = strcmpi(['123'],'12');
% TEST 26
res26 = strcmpi(['idem'],'12');
% TEST 27
res27 = strcmpi(0,'12');
% TEST 28
res28 = strcmpi(1,'12');
% TEST 29
res29 = strcmpi(['str1'],['str2','str1']);
% TEST 30
res30 = strcmpi(['str1 str2'],['str2','str1']);
% TEST 31
res31 = strcmpi(['str1','str8'],['str2','str1']);
% TEST 32
res32 = strcmpi(['123'],['str2','str1']);
% TEST 33
res33 = strcmpi(['idem'],['str2','str1']);
% TEST 34
res34 = strcmpi(0,['str2','str1']);
% TEST 35
res35 = strcmpi(1,['str2','str1']);
% TEST 36
res36 = strcmpi(['str1'],['idem']);
% TEST 37
res37 = strcmpi(['str1 str2'],['idem']);
% TEST 38
res38 = strcmpi(['str1','str8'],['idem']);
% TEST 39
res39 = strcmpi(['123'],['idem']);
% TEST 40
res40 = strcmpi(['idem'],['idem']);
% TEST 41
res41 = strcmpi(0,['idem']);
% TEST 42
res42 = strcmpi(1,['idem']);
% TEST 43
res43 = strcmpi(['str1'],0);
% TEST 44
res44 = strcmpi(['str1 str2'],0);
% TEST 45
res45 = strcmpi(['str1','str8'],0);
% TEST 46
res46 = strcmpi(['123'],0);
% TEST 47
res47 = strcmpi(['idem'],0);
% TEST 48
res48 = strcmpi(0,0);
% TEST 49
res49 = strcmpi(1,0);
% TEST 50
res50 = strcmpi(['str1'],0);
% TEST 51
res51 = strcmpi(['str1 str2'],0);
% TEST 52
res52 = strcmpi(['str1','str8'],0);
% TEST 53
res53 = strcmpi(['123'],0);
% TEST 54
res54 = strcmpi(['idem'],0);
% TEST 55
res55 = strcmpi(0,0);
% TEST 56
res56 = strcmpi(1,0);
