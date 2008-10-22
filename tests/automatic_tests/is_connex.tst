getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/is_connex_data.ref','rb');
g = make_graph('foo', 1, 3, [1,2,3,1], [2,3,1,3]);
%ans = is_connex(g);
if load_ref('%ans') then   pause,end,

g = make_graph('foo', 1, 4, [1,2,3,1], [2,3,1,3]);
%ans = is_connex(g);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
