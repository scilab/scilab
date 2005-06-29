getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/chepol_data.ref','r');
%ans = chepol(4, 'x');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
