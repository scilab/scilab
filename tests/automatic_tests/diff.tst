getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/diff_data.ref','rb');
v = (1:8)^3;
%ans = diff(v);
if load_ref('%ans') then   pause,end,

%ans = diff(v, 3);
if load_ref('%ans') then   pause,end,


A = [(1:8)^2;
  (1:8)^3;
  (1:8)^4];

%ans = diff(A, 3, 2);
if load_ref('%ans') then   pause,end,


//approximate differentiation
step = 0.001;
if load_ref('step') then   pause,end,

t = 0:step:10;
y = sin(t);
dy = diff(sin(t))/step;//approximate differentiation of sine function
%ans = norm(dy - cos(t(1:$ - 1)), %inf);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
