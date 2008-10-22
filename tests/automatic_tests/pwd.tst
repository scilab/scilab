getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pwd_data.ref','rb');
%ans = pwd();
if load_ref_nocheck('%ans') then   pause,end,

x = pwd();
if load_ref_nocheck('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
