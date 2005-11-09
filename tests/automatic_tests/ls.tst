getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ls_data.ref','r');
ls;
if load_ref('%ans') then   pause,end,

%ans = ls('SCI/macros/util/*.sci');
if load_ref('%ans') then   pause,end,

x = ls('SCI/macros/util/f*.sci');
if load_ref('x') then   pause,end,

xdel_run(winsid());

mclose(%U);
