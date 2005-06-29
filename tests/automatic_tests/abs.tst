getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/abs_data.ref','r');
%ans = abs([1,%i,-1,-%i,1 + %i]);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
