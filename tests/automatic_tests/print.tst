getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/print_data.ref','rb');
a = rand(3, 3);p = poly([1,2,3], 's');l = list(1, 'asdf', [1,2,3]);
%ans = print(%io(2), a, p, l);
if load_ref('%ans') then   pause,end,

%ans = write(%io(2), a);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
