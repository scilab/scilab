getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ones_data.ref','rb');
%ans = ones(3);
if load_ref('%ans') then   pause,end,

%ans = ones(3, 3);
if load_ref('%ans') then   pause,end,

%ans = ones(2, 3, 2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
