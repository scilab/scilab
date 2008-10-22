getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/hinf_data.ref','rb');
//example from Niconet report SLWN1999-12
//Hinf
A = [-1,0,4,5,-3,-2;
  -2,4,-7,-2,0,3;
  -6,9,-5,0,2,-1;
  -8,4,7,-1,-3,0;
  2,5,8,-9,1,-4;
  3,-5,8,0,2,-6];

B = [-3,-4,-2,1,0;
  2,0,1,-5,2;
  -5,-7,0,7,-2;
  4,-6,1,1,-2;
  -3,9,-8,0,5;
  1,-2,3,-6,-2];

C = [1,-1,2,-4,0,-3;
  -3,0,5,-1,1,1;
  -7,5,0,-8,2,-2;
  9,-3,4,0,3,7;
  0,1,-2,1,-6,-2];

D = [1,-2,-3,0,0;
  0,4,0,1,0;
  5,-3,-4,0,1;
  0,1,0,1,-3;
  0,0,1,7,1];
Gamma = 10.18425636157899;
[AK,BK,CK,DK] = hinf(A, B, C, D, 2, 2, Gamma);
if load_ref('DK') then   pause,end,
if load_ref('CK') then   pause,end,
if load_ref('BK') then   pause,end,
if load_ref('AK') then   pause,end,

xdel_run(winsid());

mclose(%U);
