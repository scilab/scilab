getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/cothm_data.ref','r');
A = [1,2;3,4];
%ans = cothm(A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
