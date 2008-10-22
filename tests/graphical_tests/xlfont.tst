getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/graphical_tests/xlfont_data.ref','rb');
// Caution : this example may not work if your system have not
//           the schoolbook bold font
if MSDOS then 
  %ans = xlfont('Century Schoolbook Bold', 10);
  if load_ref('%ans') then   pause,end,

else 
  %ans = xlfont('-adobe-new century schoolbook-bold-r-normal-*-%s-*-75-75-*-*-iso8859-1', 10);
  if load_ref('%ans') then   pause,end,

end,
%ans = xset('font', 10, 4);
if load_ref('%ans') then   pause,end,
// use Schoolbook bold at 18 pts
%ans = plot2d();
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
