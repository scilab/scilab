getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/save_data.ref','rb');
a = eye(2, 2);b = ones(a);
save('val.dat', a, b);
%ans = clear('a');
if load_ref('%ans') then   pause,end,

%ans = clear('b');
if load_ref('%ans') then   pause,end,

load('val.dat', 'a', 'b');

// sequential save into a file
fd = mopen('TMPDIR/foo', 'wb');
if load_ref_nocheck('fd') then   pause,end,

for k = 1:4,  x = k^2;%ans = save(fd, x, k);
  if load_ref('%ans') then   pause,end,
end,
%ans = mclose(fd);
if load_ref('%ans') then   pause,end,

fd = mopen('TMPDIR/foo', 'rb');
if load_ref_nocheck('fd') then   pause,end,

for i = 1:4,  load(fd, 'x', 'k');x;
  if load_ref('x') then   pause,end,
  k;
  if load_ref('k') then   pause,end,
end,
%ans = mclose(fd);
if load_ref('%ans') then   pause,end,


// appending variables to an old save file
fd = mopen('TMPDIR/foo', 'r+');
if load_ref_nocheck('fd') then   pause,end,

%ans = mseek(0, fd, 'end');
if load_ref('%ans') then   pause,end,

lst = list(1, 2, 3);
if load_ref('lst') then   pause,end,

%ans = save(fd, lst);
if load_ref('%ans') then   pause,end,

%ans = mclose(fd);
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
