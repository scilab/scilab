getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mget_data.ref','rb');
file1 = 'test1.bin';
file2 = 'test2.bin';
fd1 = mopen(file1, 'wb');
fd2 = mopen(file2, 'wb');
mput(1996, 'ull', fd1);
mput(1996, 'ull', fd2);
mclose(fd1);
mclose(fd2);

fd1 = mopen(file1, 'rb');
if 1996 ~= mget(1, 'ull', fd1) then   write(%io(2), 'Bug');end,
fd2 = mopen(file2, 'rb');
if 1996 ~= mget(1, 'ull', fd2) then   write(%io(2), 'Bug');end,
mclose(fd1);
mclose(fd2);
xdel_run(winsid());

mclose(%U);
