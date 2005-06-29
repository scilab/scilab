getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/erfc_data.ref','r');
%ans = erf([0.5,0.2]) + erfc([0.5,0.2]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
