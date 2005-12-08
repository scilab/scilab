getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/select_data.ref','rb');
while %t then
  n = round(10 * rand(1, 1));
  if load_ref('n') then   pause,end,

  select n
    case 0 then
    %ans = disp(0);
    if load_ref('%ans') then   pause,end,

  case 1 then
    %ans = disp(1);
    if load_ref('%ans') then   pause,end,

  else
    break,
  end,
end,

xdel_run(winsid());

mclose(%U);
