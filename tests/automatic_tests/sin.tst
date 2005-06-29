getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/sin_data.ref','r');
%ans = asin(sin([1,0,%i]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
