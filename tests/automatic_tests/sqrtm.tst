getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sqrtm_data.ref','rb');
x = [0,1;2,4];
if load_ref('x') then   pause,end,

w = sqrtm(x);
%ans = norm(w * w - x);
if load_ref('%ans') then   pause,end,

x(1, 2) = %i;
w = sqrtm(x);%ans = norm(w * w - x, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
