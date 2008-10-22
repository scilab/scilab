getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fun2string_data.ref','rb');
txt = fun2string(asinh, 'foo');
%ans = write(%io(2), txt, '(a)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
