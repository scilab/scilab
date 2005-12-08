getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/if_data.ref','rb');
i = 2;
if load_ref('i') then   pause,end,

for j = 1:3,
  if i == j then 
    a(i, j) = 2;
  elseif abs(i - j) == 1 then 
    a(i, j) = -1;
  else   a(i, j) = 0;
  end,
end,
xdel_run(winsid());

mclose(%U);
