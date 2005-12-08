getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/unix_data.ref','rb');
unix('ls $SCI/demos');
%ans = deff('wd=dir()', 'if MSDOS then unix(''cd>''+TMPDIR+''\path'');                else unix(''pwd>''+TMPDIR+''/path'');end      wd=read(TMPDIR+''/path'',1,1,''(a)'')');
if load_ref('%ans') then   pause,end,

wd = dir();
if load_ref_nocheck('wd') then   pause,end,

xdel_run(winsid());

mclose(%U);
