getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/getlongpathname_data.ref','r');
[longpath,bOK] = getlongpathname(SCI);
if load_ref('bOK') then   pause,end,
if load_ref_nocheck('longpath') then   pause,end,

xdel_run(winsid());

mclose(%U);
