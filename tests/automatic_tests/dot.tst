getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dot_data.ref','rb');
%ans = 1.345;
if load_ref('%ans') then   pause,end,

x = [1,2,3];%ans = x.^2 .* x;
if load_ref('%ans') then   pause,end,
// a space is required between 2 and dot
%ans = [123,456];
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
