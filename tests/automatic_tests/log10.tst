getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/log10_data.ref','r');
%ans = 10^log10([1,%i,-1,-%i]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
