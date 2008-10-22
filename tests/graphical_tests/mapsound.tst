getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/mapsound_data.ref','rb');
// At first we create 0.5 seconds of sound parameters.
t = soundsec(0.5);
// Then we generate the sound.
s = sin(440 * t) + sin(220 * t)/2 + sin(880 * t)/2;
[nr,nc] = size(t);
s(nc/2:nc) = sin(330 * t(nc/2:nc));
mapsound(s);
xdel_run(winsid());

mclose(%U);
