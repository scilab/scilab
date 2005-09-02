getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/graphic/toprint_data.ref','r');
toprint(['Test','toprint primitive'], 'Scilab page header');
scf(4);
plot();
toprint(4);
toprint(4, 'pos');

xdel_run(winsid());

mclose(%U);
