getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/dir_data.ref','rb');
dir;
if load_ref_nocheck('%ans') then   pause,end,

%ans = dir('SCI/macros/util/base*.sci');
if load_ref_nocheck('%ans') then   pause,end,

x = dir('SCI/macros/util/base*.sci');
if load_ref_nocheck('x') then   pause,end,

dt = getdate(x('date'));
%ans = mprintf('%s: %d-%d-%d %d:%d:%d\n', x('name'), dt(:, [3,2,1,7:9]));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
