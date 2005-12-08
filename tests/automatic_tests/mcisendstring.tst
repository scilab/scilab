getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/mcisendstring_data.ref','rb');
%ans = mcisendstring('open ""d:\audio.mp3"" type MPEGVideo alias MP3_Device');
if load_ref('%ans') then   pause,end,

%ans = mcisendstring('play MP3_Device');
if load_ref('%ans') then   pause,end,

%ans = mcisendstring('close MP3_Device');
if load_ref('%ans') then   pause,end,



%ans = mcisendstring('open ""d:\video.avi"" type avivideo alias AVI_Device');
if load_ref('%ans') then   pause,end,

%ans = mcisendstring('play AVI_Device from 0');
if load_ref('%ans') then   pause,end,

%ans = mcisendstring('close AVI_Device');
if load_ref('%ans') then   pause,end,


xdel_run(winsid());

mclose(%U);
