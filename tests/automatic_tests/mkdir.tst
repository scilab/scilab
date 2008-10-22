getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mkdir_data.ref','rb');
rmdir(SCI + '/Directory');//remove the directory if it exists
%ans = mkdir(SCI, 'Directory');
if load_ref_nocheck('%ans') then   pause,end,

status = mkdir(SCI, 'Directory');
if load_ref('status') then   pause,end,

[status,msg] = mkdir(SCI, 'Directory');
if load_ref('msg') then   pause,end,
if load_ref('status') then   pause,end,

%ans = rmdir(SCI + '/Directory');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
