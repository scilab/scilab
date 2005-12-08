getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/for_data.ref','rb');
n = 5;
for i = 1:n,  for j = 1:n,  a(i, j) = 1/(i + j - 1);end,end,
for j = 2:n - 1,  a(j, j) = j;end,a;
if load_ref('a') then   pause,end,

for e = eye(3, 3),  e;
  if load_ref('e') then   pause,end,
end,
for v = a,  %ans = write(6, v);
  if load_ref('%ans') then   pause,end,
end,
for j = 1:n,  v = a(:, j);
  if load_ref('v') then   pause,end,
  %ans = write(6, v);
  if load_ref('%ans') then   pause,end,
end,
for l = list(1, 2, 'example'),  l;
  if load_ref('l') then   pause,end,
end,
xdel_run(winsid());

mclose(%U);
