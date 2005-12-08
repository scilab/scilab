getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mgetl_data.ref','rb');
%ans = mgetl('SCI/scilab.star', 5);
if load_ref('%ans') then   pause,end,


%ans = mgetl('SCI/macros/elem/erf.sci');
if load_ref('%ans') then   pause,end,


fd = mopen('SCI/scilab.star', 'r');
if load_ref_nocheck('fd') then   pause,end,

%ans = mgetl(fd, 10);
if load_ref('%ans') then   pause,end,

%ans = mclose(fd);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
