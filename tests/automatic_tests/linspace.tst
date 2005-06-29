getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/linspace_data.ref','r');
%ans = linspace(1, 2, 10);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
