getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/findR_data.ref','rb');
//generate data from a given linear system
A = [0.5,0.1,-0.1,0.2;
  0.1,0,-0.1,-0.1;
  -0.4,-0.6,-0.7,-0.1;
  0.8,0,-0.6,-0.6];
B = [0.8;0.1;1;-1];
C = [1,2,-1,0];
SYS = syslin(0.1, A, B, C);
U = ones(1, 1000) + rand(1, 1000, 'normal');
Y = flts(U, SYS) + 0.5 * rand(1, 1000, 'normal');
// Compute R

[R,N,SVAL] = findR(15, Y', U');
SVAL;
if load_ref('SVAL') then   pause,end,

N;
if load_ref('N') then   pause,end,

xdel_run(winsid());

mclose(%U);
