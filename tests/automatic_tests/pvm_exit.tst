getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pvm_exit_data.ref','rb');
%ans = pvm_exit();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
