getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/calendar_data.ref','r');
%ans = calendar();
if load_ref('%ans') then   pause,end,


%ans = calendar(1973, 8);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
