getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/logspace_data.ref','rb');
%ans = logspace(1, 2, 10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
