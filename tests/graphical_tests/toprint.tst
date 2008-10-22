getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/toprint_data.ref','rb');
toprint(['Test','toprint primitive'], 'Scilab page header');
scf(4);
plot();
toprint(4);
toprint(4, 'pos');

xdel_run(winsid());

mclose(%U);
