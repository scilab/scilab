getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/sample_data.ref','rb');
X = ['a','dd','arreu';'ber','car','zon'];
if load_ref('X') then   pause,end,

s = sample(25, X);
if load_ref('s') then   pause,end,

s = sample(25, X, 'r');
if load_ref('s') then   pause,end,

s = sample(25, X, 'c');
if load_ref('s') then   pause,end,

xdel_run(winsid());

mclose(%U);
