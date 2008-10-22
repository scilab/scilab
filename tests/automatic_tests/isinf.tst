getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/isinf_data.ref','rb');
%ans = isinf([1,0.01,-%inf,%inf]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
