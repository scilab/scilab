getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/Examples_eng/other/sound_data.ref','r');
// a two channel signal
y = loadwave('SCI/demos/signal/sound/chimes.wav');
%ans = sound(y);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
