getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ldivf_data.ref','rb');
%ans = ldivf('1', '1');
if load_ref('%ans') then   pause,end,

%ans = ldivf('a', '0');
if load_ref('%ans') then   pause,end,

%ans = ldivf('a', 'x');
if load_ref('%ans') then   pause,end,

%ans = ldivf('2', '4');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
