getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/rank_data.ref','rb');
%ans = rank([1.00000000000E-80,0;0,1.00000000000E-80]);
if load_ref('%ans') then   pause,end,

%ans = rank([1,0;0,1.00000000000E-80]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
