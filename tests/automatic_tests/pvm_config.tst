getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/pvm_config_data.ref','rb');
if pvm_start() == 0 then 
  res = pvm_config();
  if load_ref('res') then   pause,end,

  %ans = pvm_halt();
  if load_ref('%ans') then   pause,end,

end,
xdel_run(winsid());

mclose(%U);
