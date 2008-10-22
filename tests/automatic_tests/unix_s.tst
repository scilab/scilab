getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/unix_s_data.ref','rb');
if MSDOS then 
  unix_s('del foo');
else 
  unix_s('rm -f foo');
end,
xdel_run(winsid());

mclose(%U);
