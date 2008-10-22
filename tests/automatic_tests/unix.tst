getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/unix_data.ref','rb');
if ~MSDOS then 
  unix('ls $SCI/demos');
end,

%ans = deff('wd=directory()', 'if MSDOS then unix(''cd>''+TMPDIR+''\path'');                else unix(''pwd>''+TMPDIR+''/path'');end      wd=read(TMPDIR+''/path'',1,1,''(a)'')');
if load_ref('%ans') then   pause,end,

wd = directory();
if load_ref_nocheck('wd') then   pause,end,


xdel_run(winsid());

mclose(%U);
