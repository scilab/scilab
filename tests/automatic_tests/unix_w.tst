getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/unix_w_data.ref','r');
if MSDOS then 
  unix_w('dir ''' + WSCI + '\demos''');
else 
  unix_w('ls $SCI/demos');
end,

xdel_run(winsid());

mclose(%U);
