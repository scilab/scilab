getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/banner_data.ref','r');
clc();%ans = banner();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
