% Test file for function int8()
% Matlab version: 7.9.0.529 (R2009b)

% TEST 1
res1 = int8([]);res1 = double(res1);
% TEST 2
res2 = int8(m2sciUnknownType([]));res2 = double(res2);
% TEST 3
res3 = int8(m2sciUnknownDims([]));res3 = double(res3);
% TEST 4
res4 = int8([1]);res4 = double(res4);
% TEST 5
res5 = int8([1,2,3]);res5 = double(res5);
% TEST 6
res6 = int8([1;2;3]);res6 = double(res6);
% TEST 7
res7 = int8([1,2,3;4,5,6]);res7 = double(res7);
% TEST 8
res8 = int8(m2sciUnknownType([1]));res8 = double(res8);
% TEST 9
res9 = int8(m2sciUnknownType([1,2,3]));res9 = double(res9);
% TEST 10
res10 = int8(m2sciUnknownType([1;2;3]));res10 = double(res10);
% TEST 11
res11 = int8(m2sciUnknownType([1,2,3;4,5,6]));res11 = double(res11);
% TEST 12
res12 = int8(m2sciUnknownDims([1]));res12 = double(res12);
% TEST 13
res13 = int8(m2sciUnknownDims([1,2,3]));res13 = double(res13);
% TEST 14
res14 = int8(m2sciUnknownDims([1;2;3]));res14 = double(res14);
% TEST 15
res15 = int8(m2sciUnknownDims([1,2,3;4,5,6]));res15 = double(res15);
% TEST 16
res16 = int8([i]);res16 = double(res16);
% TEST 17
res17 = int8([i,2i,3i]);res17 = double(res17);
% TEST 18
res18 = int8([i;2i;3i]);res18 = double(res18);
% TEST 19
res19 = int8([i,2i,3i;4i,5i,6i]);res19 = double(res19);
% TEST 20
res20 = int8(m2sciUnknownType([i]));res20 = double(res20);
% TEST 21
res21 = int8(m2sciUnknownType([i,2i,3i]));res21 = double(res21);
% TEST 22
res22 = int8(m2sciUnknownType([i;2i;3i]));res22 = double(res22);
% TEST 23
res23 = int8(m2sciUnknownType([i,2i,3i;4i,5i,6i]));res23 = double(res23);
% TEST 24
res24 = int8(m2sciUnknownDims([i]));res24 = double(res24);
% TEST 25
res25 = int8(m2sciUnknownDims([i,2i,3i]));res25 = double(res25);
% TEST 26
res26 = int8(m2sciUnknownDims([i;2i;3i]));res26 = double(res26);
% TEST 27
res27 = int8(m2sciUnknownDims([i,2i,3i;4i,5i,6i]));res27 = double(res27);
% TEST 28
res28 = int8(['s']);res28 = double(res28);
% TEST 29
res29 = int8(['str1']);res29 = double(res29);
% TEST 30
res30 = int8(['str1','str2','str3']);res30 = double(res30);
% TEST 31
res31 = int8(['str1';'str2';'str3']);res31 = double(res31);
% TEST 32
res32 = int8(['str1','str2','str3';'str4','str5','str6']);res32 = double(res32);
% TEST 33
res33 = int8(m2sciUnknownType(['s']));res33 = double(res33);
% TEST 34
res34 = int8(m2sciUnknownType(['str1']));res34 = double(res34);
% TEST 35
res35 = int8(m2sciUnknownType(['str1','str2','str3']));res35 = double(res35);
% TEST 36
res36 = int8(m2sciUnknownType(['str1';'str2';'str3']));res36 = double(res36);
% TEST 37
res37 = int8(m2sciUnknownType(['str1','str2','str3';'str4','str5','str6']));res37 = double(res37);
% TEST 38
res38 = int8(m2sciUnknownDims(['s']));res38 = double(res38);
% TEST 39
res39 = int8(m2sciUnknownDims(['str1']));res39 = double(res39);
% TEST 40
res40 = int8(m2sciUnknownDims(['str1','str2','str3']));res40 = double(res40);
% TEST 41
res41 = int8(m2sciUnknownDims(['str1';'str2';'str3']));res41 = double(res41);
% TEST 42
res42 = int8(m2sciUnknownDims(['str1','str2','str3';'str4','str5','str6']));res42 = double(res42);
% TEST 43
res43 = int8([[1]==[1]]);res43 = double(res43);
% TEST 44
res44 = int8([[1,2,3]==[1,0,3]]);res44 = double(res44);
% TEST 45
res45 = int8([[1;2;3]==[1;0;3]]);res45 = double(res45);
% TEST 46
res46 = int8([[1,2,3;4,5,6]==[1,0,3;4,5,0]]);res46 = double(res46);
% TEST 47
res47 = int8(m2sciUnknownType([[1]==[1]]));res47 = double(res47);
% TEST 48
res48 = int8(m2sciUnknownType([[1,2,3]==[1,0,3]]));res48 = double(res48);
% TEST 49
res49 = int8(m2sciUnknownType([[1;2;3]==[1;0;3]]));res49 = double(res49);
% TEST 50
res50 = int8(m2sciUnknownType([[1,2,3;4,5,6]==[1,0,3;4,5,0]]));res50 = double(res50);
% TEST 51
res51 = int8(m2sciUnknownDims([[1]==[1]]));res51 = double(res51);
% TEST 52
res52 = int8(m2sciUnknownDims([[1,2,3]==[1,0,3]]));res52 = double(res52);
% TEST 53
res53 = int8(m2sciUnknownDims([[1;2;3]==[1;0;3]]));res53 = double(res53);
% TEST 54
res54 = int8(m2sciUnknownDims([[1,2,3;4,5,6]==[1,0,3;4,5,0]]));res54 = double(res54);
% TEST 55
res55 = int8(inf);res55 = double(res55);
% TEST 56
res56 = int8(-inf);res56 = double(res56);
% TEST 57
res57 = int8(nan);res57 = double(res57);
