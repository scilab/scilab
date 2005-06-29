getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/or_data.ref','r');
%ans = or([%t,%t,%f]);
if load_ref('%ans') then   pause,end,

%ans = [%t,%t,%f] | [%f,%t,%t];
if load_ref('%ans') then   pause,end,

%ans = [%t,%t,%f] | %f;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
