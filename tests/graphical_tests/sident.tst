getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/sident_data.ref','rb');
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

S = 15;
N = 3;
METH = 1;
[R,N1] = findR(S, Y', U', METH);
[A,C,B,D,K] = sident(METH, 1, S, N, 1, R);
SYS1 = syslin(1, A, B, C, D);
SYS1('X0') = inistate(SYS1, Y', U');

Y1 = flts(U, SYS1);
xbasc_run();%ans = plot2d((1:nsmp)', [Y',Y1']);
if load_ref('%ans') then   pause,end,




METH = 2;
[R,N1,SVAL] = findR(S, Y', U', METH);
tol = 0;
t = size(U', 1) - 2 * S + 1;

[A,C,B,D,K] = sident(METH, 1, S, N, 1, R, tol, t);
if load_ref('K') then   pause,end,
if load_ref('D') then   pause,end,
if load_ref('B') then   pause,end,
if load_ref('C') then   pause,end,
if load_ref('A') then   pause,end,

SYS1 = syslin(1, A, B, C, D);
if load_ref('SYS1') then   pause,end,

SYS1('X0') = inistate(SYS1, Y', U');

Y1 = flts(U, SYS1);
xbasc_run();%ans = plot2d((1:nsmp)', [Y',Y1']);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
