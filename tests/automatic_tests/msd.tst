getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/msd_data.ref','rb');

x = [0.2113249,0.0002211,0.6653811;0.7560439,0.3303271,0.6283918];
if load_ref('x') then   pause,end,

m = msd(x);
if load_ref('m') then   pause,end,

m = msd(x, 'r');
if load_ref('m') then   pause,end,

m = msd(x, 'c');
if load_ref('m') then   pause,end,

xdel_run(winsid());

mclose(%U);
