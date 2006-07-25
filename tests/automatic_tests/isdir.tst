getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/isdir_data.ref','rb');
%ans = isdir(TMPDIR);
if load_ref('%ans') then   pause,end,

%ans = isdir('SCI/scilab.start');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
