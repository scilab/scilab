getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/disp_data.ref','rb');
%ans = disp([1,2], 3);
if load_ref('%ans') then   pause,end,

%ans = deff('[]=%t_p(l)', 'disp(l(3),l(2))');
if load_ref('%ans') then   pause,end,

%ans = disp(tlist('t', 1, 2));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
