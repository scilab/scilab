getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/unix_x_data.ref','rb');

if MSDOS then   unix_x('dir ' + '""' + WSCI + '\demos' + '""');
else   unix_x('ls $SCI/demos');end,

xdel_run(winsid());

mclose(%U);
