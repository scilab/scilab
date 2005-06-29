getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/fun2string_data.ref','r');
txt = fun2string(asinh, 'foo');
%ans = write(%io(2), txt, '(a)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
