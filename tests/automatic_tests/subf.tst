getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/subf_data.ref','rb');
%ans = subf('0', 'a');
if load_ref('%ans') then   pause,end,

%ans = subf('2', '1');
if load_ref('%ans') then   pause,end,

%ans = subf('a', '0');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
