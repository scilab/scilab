getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/or_data.ref','r');
%ans = or([%t,%t,%f]);
if load_ref('%ans') then   pause,end,

%ans = [%t,%t,%f] | [%f,%t,%t];
if load_ref('%ans') then   pause,end,

%ans = [%t,%t,%f] | %f;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
