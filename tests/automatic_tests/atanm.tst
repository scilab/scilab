getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/atanm_data.ref','rb');
%ans = tanm(atanm([1,2;3,4]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
