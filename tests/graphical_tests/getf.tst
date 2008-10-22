getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/getf_data.ref','rb');
%ans = getf('SCI/macros/xdess/plot.sci');
if load_ref('%ans') then   pause,end,


%ans = getf('SCI/macros/xdess/plot.sci');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
