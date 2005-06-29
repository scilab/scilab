getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/copy_data.ref','r');


xdel_run(winsid());

mclose(%U);
