getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/wavread_data.ref','rb');
%ans = wavread('SCI/demos/signal/sound/chimes.wav', 'size');
if load_ref('%ans') then   pause,end,

[y,Fs,bits] = wavread('SCI/demos/signal/sound/chimes.wav');Fs;
if load_ref('Fs') then   pause,end,
bits;
if load_ref('bits') then   pause,end,

%ans = subplot(2, 1, 1);
if load_ref('%ans') then   pause,end,

%ans = plot2d(y(1, :));
if load_ref('%ans') then   pause,end,
// first channel
%ans = subplot(2, 1, 2);
if load_ref('%ans') then   pause,end,

%ans = plot2d(y(2, :));
if load_ref('%ans') then   pause,end,
// second channel
y = wavread('SCI/demos/signal/sound/chimes.wav', [1,5]);
if load_ref('y') then   pause,end,
//the first five samples
xdel_run(winsid());

mclose(%U);
