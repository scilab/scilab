getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/mulf_data.ref','r');
%ans = mulf('1', 'a');
if load_ref('%ans') then   pause,end,

%ans = mulf('0', 'a');
if load_ref('%ans') then   pause,end,

%ans = 'a' + 'b';
if load_ref('%ans') then   pause,end,
//Caution...
xdel_run(winsid());

mclose(%U);
