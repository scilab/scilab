getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/datenum_data.ref','r');
%ans = datenum();
if load_ref('%ans') then   pause,end,


%ans = datenum(1973, 8, 4);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
