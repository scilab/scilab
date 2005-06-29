getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/gammaln_data.ref','r');
%ans = gammaln(0.5);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
