getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rand_data.ref','rb');
x = rand(10, 10, 'uniform');
if load_ref('x') then   pause,end,

%ans = rand('normal');
if load_ref('%ans') then   pause,end,

%ans = rand('info');
if load_ref('%ans') then   pause,end,

y = rand(x, 'normal');
x = rand(2, 2, 2);
if load_ref('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
