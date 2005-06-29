getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/cotg_data.ref','r');
x = [1,%i];
%ans = cotg(x) - cos(x) ./ sin(x);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
