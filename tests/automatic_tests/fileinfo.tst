getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fileinfo_data.ref','rb');
w = fileinfo(SCI + '/scilab.star');
if load_ref_nocheck('w') then   pause,end,

%ans = getdate(w(6));
if load_ref_nocheck('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
