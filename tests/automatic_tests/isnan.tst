getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/isnan_data.ref','r');
%ans = isnan([1,0.01,-%nan,%inf - %inf]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
