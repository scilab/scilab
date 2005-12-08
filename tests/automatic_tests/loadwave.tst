getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/loadwave_data.ref','rb');
// At first we create 0.5 seconds of sound parameters.
t = soundsec(0.5);
// Then we generate the sound: a two channels sound.
s = [sin(2 * %pi * 440 * t);sin(2 * %pi * 350 * t)];
savewave(TMPDIR + '/foo.wav', s);
s1 = loadwave(TMPDIR + '/foo.wav');
%ans = max(abs(s1 - s));
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
