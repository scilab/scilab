getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/cosm_data.ref','r');
A = [1,2;3,4];
if load_ref('A') then   pause,end,

%ans = cosm(A) - 0.5 * (expm(%i * A) + expm(-%i * A));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
