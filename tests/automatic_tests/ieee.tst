getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/ieee_data.ref','rb');
ieee(1);%ans = 1/0;
if load_ref('%ans') then   pause,end,

ieee(2);%ans = 1/0;
if load_ref('%ans') then   pause,end,
%ans = log(0);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
