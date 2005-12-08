getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/soundsec_data.ref','rb');
// At first we create 0.5 seconds of sound parameters.
t = soundsec(0.5);
// Then we generate the sound.
s = sin(2 * %pi * 440 * t);
analyze(s, 200, 600, 22050);
xdel_run(winsid());

mclose(%U);
