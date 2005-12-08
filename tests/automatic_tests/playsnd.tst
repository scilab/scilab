getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/playsnd_data.ref','rb');
// a two channel signal
y = loadwave('SCI/demos/signal/sound/chimes.wav');
%ans = playsnd(y);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
