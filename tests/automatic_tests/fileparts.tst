getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fileparts_data.ref','rb');
[path,fname,extension] = fileparts('SCI/scilab.star');
if load_ref('extension') then   pause,end,
if load_ref('fname') then   pause,end,
if load_ref('path') then   pause,end,

%ans = fileparts('SCI/scilab.star', 'extension');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
