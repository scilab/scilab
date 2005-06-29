getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/log_data.ref','r');
%ans = exp(log([1,%i,-1,-%i]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
