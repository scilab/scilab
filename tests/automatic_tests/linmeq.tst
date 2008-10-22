getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/linmeq_data.ref','rb');
//(1a)
n = 40;m = 30;
A = rand(n, n);C = rand(n, m);B = rand(m, m);
X = linmeq(1, A, B, C);
%ans = norm(A * X + X * B - C, 1);
if load_ref('%ans') then   pause,end,

//(1b)
flag = [1,0,0];
if load_ref('flag') then   pause,end,

X = linmeq(1, A, B, C, flag);
%ans = norm(A * X * B + X - C, 1);
if load_ref('%ans') then   pause,end,

//(2a)
A = rand(n, n);C = rand(A);C = C + C';
X = linmeq(2, A, C);
%ans = norm(A' * X + X * A - C, 1);
if load_ref('%ans') then   pause,end,

//(2b)
X = linmeq(2, A, C, [1,0]);
%ans = norm(A' * X * A - X - C, 1);
if load_ref('%ans') then   pause,end,

//(3a)
A = rand(n, n);
A = A - (max(real(spec(A))) + 1) * eye();//shift eigenvalues
C = rand(A);
X = linmeq(3, A, C);
%ans = norm(A' * X' * X + X' * X * A + C' * C, 1);
if load_ref('%ans') then   pause,end,

//(3b)
A = [-0.02,0.02,-0.1,0.02,-0.03,0.12;
  0.02,0.14,0.12,-0.1,-0.02,-0.14;
  -0.1,0.12,0.05,0.03,-0.04,-0.04;
  0.02,-0.1,0.03,-0.06,0.08,0.11;
  -0.03,-0.02,-0.04,0.08,0.14,-0.07;
  0.12,-0.14,-0.04,0.11,-0.07,0.04];
if load_ref('A') then   pause,end,


C = rand(A);
X = linmeq(3, A, C, [1,0]);
%ans = norm(A' * X' * X * A - X' * X + C' * C, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
