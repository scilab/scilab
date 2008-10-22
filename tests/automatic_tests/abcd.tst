getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/abcd_data.ref','rb');
A = diag([1,2,3]);B = [1;1;1];C = [2,2,2];
sys = syslin('c', A, B, C);
%ans = sys('A');
if load_ref('%ans') then   pause,end,

%ans = sys('C');
if load_ref('%ans') then   pause,end,

[A1,B1,C1,D1] = abcd(sys);
A1;
if load_ref('A1') then   pause,end,

systf = ss2tf(sys);
[a,b,c,d] = abcd(systf);
if load_ref('d') then   pause,end,
if load_ref('c') then   pause,end,
if load_ref('b') then   pause,end,
if load_ref('a') then   pause,end,

%ans = spec(a);
if load_ref('%ans') then   pause,end,

%ans = c * b - C * B;
if load_ref('%ans') then   pause,end,

%ans = c * a * b - C * A * B;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
