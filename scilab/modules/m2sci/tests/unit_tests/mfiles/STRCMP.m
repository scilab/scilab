% Test file for function strcmp()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = strcmp(['str1'],'s');
% TEST 2
res2 = strcmp(['str1 str2'],'s');
% TEST 3
res3 = strcmp(['str1','str8'],'s');
% TEST 4
res4 = strcmp(['123'],'s');
% TEST 5
res5 = strcmp(['idem'],'s');
% TEST 6
res6 = strcmp(0,'s');
% TEST 7
res7 = strcmp(1,'s');
% TEST 8
res8 = strcmp(['str1'],'str14');
% TEST 9
res9 = strcmp(['str1 str2'],'str14');
% TEST 10
res10 = strcmp(['str1','str8'],'str14');
% TEST 11
res11 = strcmp(['123'],'str14');
% TEST 12
res12 = strcmp(['idem'],'str14');
% TEST 13
res13 = strcmp(0,'str14');
% TEST 14
res14 = strcmp(1,'str14');
% TEST 15
res15 = strcmp(['str1'],['8']);
% TEST 16
res16 = strcmp(['str1 str2'],['8']);
% TEST 17
res17 = strcmp(['str1','str8'],['8']);
% TEST 18
res18 = strcmp(['123'],['8']);
% TEST 19
res19 = strcmp(['idem'],['8']);
% TEST 20
res20 = strcmp(0,['8']);
% TEST 21
res21 = strcmp(1,['8']);
% TEST 22
res22 = strcmp(['str1'],'12');
% TEST 23
res23 = strcmp(['str1 str2'],'12');
% TEST 24
res24 = strcmp(['str1','str8'],'12');
% TEST 25
res25 = strcmp(['123'],'12');
% TEST 26
res26 = strcmp(['idem'],'12');
% TEST 27
res27 = strcmp(0,'12');
% TEST 28
res28 = strcmp(1,'12');
% TEST 29
res29 = strcmp(['str1'],['str2','str1']);
% TEST 30
res30 = strcmp(['str1 str2'],['str2','str1']);
% TEST 31
res31 = strcmp(['str1','str8'],['str2','str1']);
% TEST 32
res32 = strcmp(['123'],['str2','str1']);
% TEST 33
res33 = strcmp(['idem'],['str2','str1']);
% TEST 34
res34 = strcmp(0,['str2','str1']);
% TEST 35
res35 = strcmp(1,['str2','str1']);
% TEST 36
res36 = strcmp(['str1'],['idem']);
% TEST 37
res37 = strcmp(['str1 str2'],['idem']);
% TEST 38
res38 = strcmp(['str1','str8'],['idem']);
% TEST 39
res39 = strcmp(['123'],['idem']);
% TEST 40
res40 = strcmp(['idem'],['idem']);
% TEST 41
res41 = strcmp(0,['idem']);
% TEST 42
res42 = strcmp(1,['idem']);
% TEST 43
res43 = strcmp(['str1'],0);
% TEST 44
res44 = strcmp(['str1 str2'],0);
% TEST 45
res45 = strcmp(['str1','str8'],0);
% TEST 46
res46 = strcmp(['123'],0);
% TEST 47
res47 = strcmp(['idem'],0);
% TEST 48
res48 = strcmp(0,0);
% TEST 49
res49 = strcmp(1,0);
% TEST 50
res50 = strcmp(['str1'],0);
% TEST 51
res51 = strcmp(['str1 str2'],0);
% TEST 52
res52 = strcmp(['str1','str8'],0);
% TEST 53
res53 = strcmp(['123'],0);
% TEST 54
res54 = strcmp(['idem'],0);
% TEST 55
res55 = strcmp(0,0);
% TEST 56
res56 = strcmp(1,0);
