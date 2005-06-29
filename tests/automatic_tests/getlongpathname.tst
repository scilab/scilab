getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/getlongpathname_data.ref','r');
[longpath,bOK] = getlongpathname(SCI);
if load_ref('bOK') then   pause,end,
if load_ref('longpath') then   pause,end,

xdel_run(winsid());

mclose(%U);
