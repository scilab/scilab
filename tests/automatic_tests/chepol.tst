getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/chepol_data.ref','r');
%ans = chepol(4, 'x');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
