getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/log_data.ref','rb');
%ans = exp(log([1,%i,-1,-%i]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
