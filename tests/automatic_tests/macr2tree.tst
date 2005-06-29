getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/macr2tree_data.ref','r');
tree = macr2tree(help);
txt = tree2code(tree, %T);
%ans = write(%io(2), txt, '(a)');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
