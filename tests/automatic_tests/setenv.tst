getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/setenv_data.ref','rb');
%ans = setenv('toto', 'example');
if load_ref('%ans') then   pause,end,

%ans = getenv('toto');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
