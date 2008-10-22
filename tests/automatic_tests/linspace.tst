getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/linspace_data.ref','rb');
%ans = linspace(1, 2, 10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
