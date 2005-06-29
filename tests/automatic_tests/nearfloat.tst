getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/nearfloat_data.ref','r');
%ans = format('e', 22);
if load_ref('%ans') then   pause,end,

%ans = nearfloat('succ', 1) - 1;
if load_ref('%ans') then   pause,end,

%ans = 1 - nearfloat('pred', 1);
if load_ref('%ans') then   pause,end,

%ans = format('v');
if load_ref('%ans') then   pause,end,
//reset default format
xdel_run(winsid());

mclose(%U);
