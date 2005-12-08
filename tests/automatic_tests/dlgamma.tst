getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dlgamma_data.ref','rb');
%ans = dlgamma(0.5);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
