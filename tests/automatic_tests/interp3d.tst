getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/interp3d_data.ref','r');
// see the examples of the splin3d help page
xdel_run(winsid());

mclose(%U);
