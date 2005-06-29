getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/getd_data.ref','r');
%ans = getd('SCI/macros/auto');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
