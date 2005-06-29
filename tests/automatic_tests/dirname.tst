getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/dirname_data.ref','r');
files = dirname('SCI/macros/util/poo.sci');
if load_ref('files') then   pause,end,

files = dirname('SCI/macros\util/poo.sci');
if load_ref('files') then   pause,end,

files = dirname('SCI/macros\util/poo.sci.k');
if load_ref('files') then   pause,end,

xdel_run(winsid());

mclose(%U);
