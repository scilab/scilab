getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/svd_data.ref','rb');
X = rand(4, 2) * rand(2, 4);
if load_ref('X') then   pause,end,

%ans = svd(X);
if load_ref('%ans') then   pause,end,

%ans = sqrt(spec(X * X'));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
