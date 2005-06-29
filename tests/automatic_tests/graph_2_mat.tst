getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/graph_2_mat_data.ref','r');
g = load_graph(SCI + '/demos/metanet/colored');
a = graph_2_mat(g);
if load_ref('a') then   pause,end,

a = graph_2_mat(g, 'node-node');
if load_ref('a') then   pause,end,

xdel_run(winsid());

mclose(%U);
