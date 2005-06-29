getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('/usr/local/lib/scilab/tests/Examples/other/continue_data.ref','r');
for k = 1:10,  K = k;if (k > 2) & (k <= 8) then   continue,%ans = disp('hello');
    if load_ref('%ans') then   pause,end,
  end,k;
  if load_ref('k') then   pause,end,
end,

for j = 1:2,
  x = [];
  for k = 1:10,  if (k > (j + 1)) & (k <= 8) then   continue,end,x = [x,k];end,
  x;
  if load_ref('x') then   pause,end,

end,
xdel_run(winsid());

mclose(%U);
