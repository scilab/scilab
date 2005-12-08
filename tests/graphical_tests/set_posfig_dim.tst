getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/set_posfig_dim_data.ref','rb');
// make a figure with a specific shape
f = scf(0);f('figure_size') = [800,300];
%ans = plot2d();
if load_ref('%ans') then   pause,end,

%ans = set_posfig_dim(f.figure_size(1), f.figure_size(2));
if load_ref('%ans') then   pause,end,

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

xdel_run(winsid());

mclose(%U);
