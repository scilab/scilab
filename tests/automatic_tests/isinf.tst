getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/isinf_data.ref','r');
%ans = isinf([1,0.01,-%inf,%inf]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
