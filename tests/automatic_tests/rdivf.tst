getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rdivf_data.ref','rb');
%ans = ldivf('c', 'd');
if load_ref('%ans') then   pause,end,

%ans = ldivf('1', '2');
if load_ref('%ans') then   pause,end,

%ans = ldivf('a', '0');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
