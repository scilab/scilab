getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/printf_data.ref','rb');
%ans = printf('Result is:\nalpha=%f', 0.535);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
