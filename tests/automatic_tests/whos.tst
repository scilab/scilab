getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/whos_data.ref','r');
%ans = whos();
if load_ref('%ans') then   pause,end,


%ans = whos('-type', 'boolean');
if load_ref('%ans') then   pause,end,


%ans = whos('-name', '%');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
