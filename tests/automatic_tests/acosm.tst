getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/acosm_data.ref','r');
A = [1,2;3,4];
%ans = cosm(acosm(A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
