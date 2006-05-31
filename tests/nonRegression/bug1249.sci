// Non-regression test file for bug 1249
// Author : Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 21 Mar 2005

mode(-1);
clear;

M = calendar(2005,3);

M1ref = ['Mar 2005'];
M2ref = ['   M      Tu     W      Th     F     Sat     Sun'];
M3ref = [0,1,2,3,4,5,6;7,8,9,10,11,12,13;14,15,16,17,18,19,20;21,22,23,24,25,26,27;28,29,30,31,0,0,0;0,0,0,0,0,0,0];

Mref = list(M1ref,M2ref,M3ref );

if or((Mref<>M) <> [%F,%F,%F]) then
	affich_result(%F,1249);
else
	affich_result(%T,1249);
end

clear