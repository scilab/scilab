getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/lasterror_data.ref','r');
ierr = execstr('a=zzzzzzz', 'errcatch');
if load_ref('ierr') then   pause,end,

if ierr > 0 then   %ans = disp(lasterror());
  if load_ref('%ans') then   pause,end,
end,
xdel_run(winsid());

mclose(%U);
