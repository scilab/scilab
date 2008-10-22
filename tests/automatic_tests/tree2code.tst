getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/tree2code_data.ref','rb');
tree = macr2tree(cosh);
txt = tree2code(tree, %T);
write(%io(2), txt, '(a)');
xdel_run(winsid());

mclose(%U);
