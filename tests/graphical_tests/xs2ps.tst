getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xs2ps_data.ref','rb');
%ans = scf(0);
if load_ref('%ans') then   pause,end,

%ans = plot2d();
if load_ref('%ans') then   pause,end,

// Postcript export
filename = 'foo';// ! no extension
%ans = xs2ps(0, filename);
if load_ref('%ans') then   pause,end,

//add the Postscript file header and create <filename>.eps file
if MSDOS then 
  %ans = unix_s(pathconvert('""' + WSCI + '/bin/BEpsf' + '""', %f) + ' -p ' + filename);
  if load_ref('%ans') then   pause,end,

else 
  %ans = unix_s(pathconvert('SCI/bin/BEpsf', %f) + ' -p ' + filename);
  if load_ref('%ans') then   pause,end,

end,
// Gif export
%ans = xs2gif(0, 'foo.gif');
if load_ref('%ans') then   pause,end,

// ppm export
%ans = xs2ppm(0, 'foo.ppm');
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
