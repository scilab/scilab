getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/texprint_data.ref','r');
s = poly(0, 's');
%ans = texprint([1/s,s^2]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
