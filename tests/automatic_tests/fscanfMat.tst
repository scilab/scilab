getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/fscanfMat_data.ref','rb');
fd = mopen(TMPDIR + '/Mat', 'w');
mfprintf(fd, 'Some text.....\n');
mfprintf(fd, 'Some text again\n');
a = rand(6, 6);
for i = 1:6,
  for j = 1:6,  mfprintf(fd, '%5.2f ', a(i, j));end,
  mfprintf(fd, '\n');
end,
mclose(fd);
a1 = fscanfMat(TMPDIR + '/Mat');
if load_ref('a1') then   pause,end,

xdel_run(winsid());

mclose(%U);
