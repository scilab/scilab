getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/thrownan_data.ref','rb');

x = [0.2113249,%nan,0.6653811;0.7560439,0.3303271,0.6283918];
if load_ref('x') then   pause,end,

[nonan,numb] = thrownan(x);
if load_ref('numb') then   pause,end,
if load_ref('nonan') then   pause,end,


xdel_run(winsid());

mclose(%U);
