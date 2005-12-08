getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/or_data.ref','rb');
%ans = or([%t,%t,%f]);
if load_ref('%ans') then   pause,end,

%ans = [%t,%t,%f] | [%f,%t,%t];
if load_ref('%ans') then   pause,end,

%ans = [%t,%t,%f] | %f;
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
