getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/write_data.ref','rb');
if MSDOS then   unix('del asave');
else   unix('rm -f asave');end,
A = rand(5, 3);write('asave', A);A = read('asave', 5, 3);
%ans = write(%io(2), A, '('' | '',3(f10.3,'' | ''))');
if load_ref('%ans') then   pause,end,

%ans = write(%io(2), string(1:10));
if load_ref('%ans') then   pause,end,

%ans = write(%io(2), strcat(string(1:10), ','));
if load_ref('%ans') then   pause,end,

%ans = write(%io(2), 1:10, '(10(i2,3x))');
if load_ref('%ans') then   pause,end,


if MSDOS then   unix('del foo');
else   unix('rm -f foo');end,
%ans = write('foo', A);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
