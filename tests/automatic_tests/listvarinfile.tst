getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/listvarinfile_data.ref','rb');
a = eye(2, 2);b = int16(ones(a));c = rand(2, 3, 3);
%ans = save('vals.dat', a, b, c);
if load_ref('%ans') then   pause,end,

%ans = listvarinfile('vals.dat');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
