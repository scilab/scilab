getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mfile2sci_data.ref','rb');
// Create a simple M-file
write(TMPDIR + '/rot90.m', ['function B = rot90(A,k)';
  '[m,n] = size(A);';
  'if nargin == 1';
  '    k = 1;';
  'else';
  '    k = rem(k,4);';
  '    if k < 0';
  '        k = k + 4;';
  '    end';
  'end';
  'if k == 1';
  '    A = A.'';';
  '    B = A(n:-1:1,:);';
  'elseif k == 2';
  '    B = A(m:-1:1,n:-1:1);';
  'elseif k == 3';
  '    B = A(m:-1:1,:);';
  '    B = B.'';';
  'else';
  '    B = A;';
  'end']);
// Convert it to scilab
%ans = mfile2sci(TMPDIR + '/rot90.m', TMPDIR);
if load_ref('%ans') then   pause,end,

// Show the new code
%ans = write(%io(2), read(TMPDIR + '/rot90.sci', -1, 1, '(a)'));
if load_ref('%ans') then   pause,end,

// get it into scilab
%ans = getf(TMPDIR + '/rot90.sci');
if load_ref('%ans') then   pause,end,

// Execute it
m = rand(4, 2);%ans = rot90(m, 1);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
