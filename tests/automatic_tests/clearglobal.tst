getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/clearglobal_data.ref','rb');
%ans = global('a', 'b', 'c');
if load_ref('%ans') then   pause,end,

a = 1;b = 2;c = 3;
%ans = who('global');
if load_ref('%ans') then   pause,end,

%ans = clearglobal('b');
if load_ref('%ans') then   pause,end,

%ans = who('global');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
