getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/file_data.ref','rb');
u = file('open', TMPDIR + '/foo', 'unknown');
if load_ref('u') then   pause,end,

for k = 1:4,
  a = rand(1, 4);
  if load_ref('a') then   pause,end,

  %ans = write(u, a);
  if load_ref('%ans') then   pause,end,

end,
%ans = file('rewind', u);
if load_ref('%ans') then   pause,end,

x = read(u, 2, 4);
if load_ref('x') then   pause,end,

%ans = file('close', u);
if load_ref('%ans') then   pause,end,

//
u1 = file('open', TMPDIR + '/foo', 'unknown');
if load_ref('u1') then   pause,end,

u2 = mopen(TMPDIR + '/foo1', 'wb');
if load_ref('u2') then   pause,end,

[units,typs,nams] = file();
if load_ref_nocheck('nams') then   pause,end,
if load_ref('typs') then   pause,end,
if load_ref('units') then   pause,end,

file('close', u1);
mclose(u2);
xdel_run(winsid());

mclose(%U);
