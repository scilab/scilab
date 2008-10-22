getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/cotg_data.ref','rb');
x = [1,%i];
%ans = cotg(x) - cos(x) ./ sin(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
