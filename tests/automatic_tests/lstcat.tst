getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/lstcat_data.ref','r');
%ans = lstcat(list(1, 2, 3), 33, list('foo', %s));
if load_ref('%ans') then   pause,end,

%ans = lstcat(1, 2, 3);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
