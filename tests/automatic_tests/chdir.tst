getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/chdir_data.ref','r');
chdir(TMPDIR);
%ans = pwd();
if load_ref_nocheck('%ans') then   pause,end,

cd();
if load_ref_nocheck('%ans') then   pause,end,

%ans = cd('SCI');
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
