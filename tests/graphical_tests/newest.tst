getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/newest_data.ref','rb');
%ans = newest('SCI/macros/xdess/bode.sci', 'SCI/macros/xdess/bode.bin');
if load_ref('%ans') then   pause,end,

%ans = newest('SCI/macros/xdess/bode.' + ['sci','bin']);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
