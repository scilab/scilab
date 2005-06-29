getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/fileinfo_data.ref','r');
w = fileinfo(SCI + '/scilab.star');
if load_ref('w') then   pause,end,

%ans = getdate(w(6));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
