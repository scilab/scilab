getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/printf_data.ref','r');
%ans = printf('Result is:\nalpha=%f', 0.535);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
