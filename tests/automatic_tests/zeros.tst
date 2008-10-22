getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/zeros_data.ref','rb');
%ans = zeros(3);
if load_ref('%ans') then   pause,end,

%ans = zeros(3, 3);
if load_ref('%ans') then   pause,end,

%ans = zeros(2, 3, 2);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
