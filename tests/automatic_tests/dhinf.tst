getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dhinf_data.ref','rb');
//example from Niconet report SLWN1999-12
//Hinf
A = [-0.7,0,0.3,0,-0.5,-0.1;
  -0.6,0.2,-0.4,-0.3,0,0;
  -0.5,0.7,-0.1,0,0,-0.8;
  -0.7,0,0,-0.5,-1,0;
  0,0.3,0.6,-0.9,0.1,-0.4;
  0.5,-0.8,0,0,0.2,-0.9];
B = [-1,-2,-2,1,0;
  1,0,1,-2,1;
  -3,-4,0,2,-2;
  1,-2,1,0,-1;
  0,1,-2,0,3;
  1,0,3,-1,-2];
C = [1,-1,2,-2,0,-3;
  -3,0,1,-1,1,0;
  0,2,0,-4,0,-2;
  1,-3,0,0,3,1;
  0,1,-2,1,0,-2];
D = [1,-1,-2,0,0;
  0,1,0,1,0;
  2,-1,-3,0,1;
  0,1,0,1,-1;
  0,0,1,2,1];

ncon = 2;
if load_ref('ncon') then   pause,end,

nmeas = 2;
if load_ref('nmeas') then   pause,end,

gam = 111.3;
[AK,BK,CK,DK] = dhinf(A, B, C, D, ncon, nmeas, gam);
if load_ref('DK') then   pause,end,
if load_ref('CK') then   pause,end,
if load_ref('BK') then   pause,end,
if load_ref('AK') then   pause,end,

xdel_run(winsid());

mclose(%U);
