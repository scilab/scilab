getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/findAC_data.ref','rb');
//generate data from a given linear system
A = [0.5,0.1,-0.1,0.2;
  0.1,0,-0.1,-0.1;
  -0.4,-0.6,-0.7,-0.1;
  0.8,0,-0.6,-0.6];
B = [0.8;0.1;1;-1];
C = [1,2,-1,0];
SYS = syslin(0.1, A, B, C);
nsmp = 100;
U = prbs_a(nsmp, nsmp/5);
Y = flts(U, SYS) + 0.3 * rand(1, nsmp, 'normal');

// Compute R
S = 15;L = 1;
[R,N,SVAL] = findR(S, Y', U');

N = 3;
METH = 3;TOL = -1;
[A,C] = findAC(S, N, L, R, METH, TOL);
xdel_run(winsid());

mclose(%U);
