getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/acos_data.ref','r');
x = [1,%i,-1,-%i];
if load_ref('x') then   pause,end,

%ans = cos(acos(x));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
