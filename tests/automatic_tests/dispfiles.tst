getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dispfiles_data.ref','rb');
%ans = dispfiles();
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
