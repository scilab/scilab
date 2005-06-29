getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/isdir_data.ref','r');
%ans = isdir(TMPDIR);
if load_ref('%ans') then   pause,end,

%ans = isdir('SCI/scilab.star');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
