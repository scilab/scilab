getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/setenv_data.ref','r');
%ans = setenv('toto', 'example');
if load_ref('%ans') then   pause,end,

%ans = getenv('toto');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
