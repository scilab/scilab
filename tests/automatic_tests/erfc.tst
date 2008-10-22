getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/erfc_data.ref','rb');
%ans = erf([0.5,0.2]) + erfc([0.5,0.2]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
