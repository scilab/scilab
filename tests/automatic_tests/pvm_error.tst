getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/pvm_error_data.ref','r');
if pvm_start() == 0 then 
  a = pvm_error(0);
  if load_ref('a') then   pause,end,

  %ans = pvm_halt();
  if load_ref('%ans') then   pause,end,

end,
xdel_run(winsid());

mclose(%U);
