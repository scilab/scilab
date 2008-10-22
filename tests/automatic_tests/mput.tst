getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mput_data.ref','rb');
filen = 'test.bin';
mopen(filen, 'wb');
mput(1996, 'l');mput(1996, 'i');mput(1996, 's');mput(98, 'c');
// force little-endian
mput(1996, 'll');mput(1996, 'il');mput(1996, 'sl');mput(98, 'cl');
// force big-endian
mput(1996, 'lb');mput(1996, 'ib');mput(1996, 'sb');mput(98, 'cb');
//
mclose();
mopen(filen, 'rb');
if 1996 ~= mget(1, 'l') then   pause,end,
if 1996 ~= mget(1, 'i') then   pause,end,
if 1996 ~= mget(1, 's') then   pause,end,
if 98 ~= mget(1, 'c') then   pause,end,
// force little-endian
if 1996 ~= mget(1, 'll') then   pause,end,
if 1996 ~= mget(1, 'il') then   pause,end,
if 1996 ~= mget(1, 'sl') then   pause,end,
if 98 ~= mget(1, 'cl') then   pause,end,
// force big-endian
if 1996 ~= mget(1, 'lb') then   pause,end,
if 1996 ~= mget(1, 'ib') then   pause,end,
if 1996 ~= mget(1, 'sb') then   pause,end,
if 98 ~= mget(1, 'cb') then   pause,end,
//
mclose();
xdel_run(winsid());

mclose(%U);
