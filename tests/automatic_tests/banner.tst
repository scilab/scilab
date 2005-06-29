getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/banner_data.ref','r');
clc();%ans = banner();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
