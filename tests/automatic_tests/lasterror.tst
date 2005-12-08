getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lasterror_data.ref','rb');
ierr = execstr('a=zzzzzzz', 'errcatch');
if load_ref('ierr') then   pause,end,

if ierr > 0 then   %ans = disp(lasterror());
  if load_ref('%ans') then   pause,end,
end,
xdel_run(winsid());

mclose(%U);
