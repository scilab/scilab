getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mvvacov_data.ref','rb');

x = [0.2113249,0.0002211,0.6653811;0.7560439,0.4453586,0.6283918];
if load_ref('x') then   pause,end,

v = mvvacov(x);
if load_ref('v') then   pause,end,

xdel_run(winsid());

mclose(%U);
