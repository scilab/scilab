getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/tokenpos_data.ref','r');
str = 'This is a character string';
kdf = tokenpos(str);
if load_ref('kdf') then   pause,end,

first = part(str, kdf(1, 1):kdf(1, 2));
if load_ref('first') then   pause,end,

xdel_run(winsid());

mclose(%U);
