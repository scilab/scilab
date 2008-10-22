getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fft2_data.ref','rb');
//Comparison with explicit formula
a = [1,2,3;4,5,6;7,8,9;10,11,12];
if load_ref('a') then   pause,end,

m = size(a, 1);
if load_ref('m') then   pause,end,

n = size(a, 2);
if load_ref('n') then   pause,end,

// fourier transform along the rows
for i = 1:n,
  a1(eye(), i) = exp(-2 * %i * %pi * ((0:m - 1)') .*. (0:m - 1)/m) * a(:, i);
  if load_ref('a1') then   pause,end,

end,
// fourier transform along the columns
for j = 1:m,
  a2temp = exp(-2 * %i * %pi * ((0:n - 1)') .*. (0:n - 1)/n) * (a1(j, :).');
  if load_ref('a2temp') then   pause,end,

  a2(j, eye()) = a2temp.';
  if load_ref('a2') then   pause,end,

end,
%ans = norm(a2 - fft2(a));
if load_ref('%ans') then   pause,end,




xdel_run(winsid());

mclose(%U);
