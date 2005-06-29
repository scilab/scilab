getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/sinh_data.ref','r');
%ans = asinh(sinh([0,1,%i]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
