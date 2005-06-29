getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/basename_data.ref','r');
files = basename('SCI/macros/util/poo.sci');
if load_ref('files') then   pause,end,

files = basename('SCI/macros\util/poo.sci');
if load_ref('files') then   pause,end,

files = basename('SCI/macros\util/poo.sci.k');
if load_ref('files') then   pause,end,

xdel_run(winsid());

mclose(%U);
