getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/emptystr_data.ref','r');
x = emptystr();for k = 1:10,  x = x + ',' + string(k);end,
xdel_run(winsid());

mclose(%U);
