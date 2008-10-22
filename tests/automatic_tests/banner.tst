getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/banner_data.ref','rb');
clc();%ans = banner();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
