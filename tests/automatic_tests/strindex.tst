getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/strindex_data.ref','r');
k = strindex('SCI/demos/scicos', '/');
if load_ref('k') then   pause,end,

k = strindex('SCI/demos/scicos', 'SCI/');
if load_ref('k') then   pause,end,

k = strindex('SCI/demos/scicos', '!');
if load_ref('k') then   pause,end,

k = strindex('aaaaa', 'aa');
if load_ref('k') then   pause,end,

k = strindex('SCI/demos/scicos', ['SCI','sci']);
if load_ref('k') then   pause,end,

[k,w] = strindex('1+3*abc/2.33', ['+','-','*','/']);
if load_ref('w') then   pause,end,
if load_ref('k') then   pause,end,

xdel_run(winsid());

mclose(%U);
