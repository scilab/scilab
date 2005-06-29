getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/atanm_data.ref','r');
%ans = tanm(atanm([1,2;3,4]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
