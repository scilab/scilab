getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/getenv_data.ref','rb');
%ans = getenv('SCI');
if load_ref_nocheck('%ans') then   pause,end,

%ans = getenv('FOO', 'foo');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
