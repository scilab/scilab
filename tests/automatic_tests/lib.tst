getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/lib_data.ref','rb');
//define some variables
function z = myplus(x, y)
  , z = x + y,
endfunction
if load_ref('%ans') then   pause,end,

function z = yourplus(x, y)
  , x = x - y,
endfunction
if load_ref('%ans') then   pause,end,

A = 1:10;

//create the *.bin files in libdir
libdir = TMPDIR;
if load_ref_nocheck('libdir') then   pause,end,

save(libdir + '/myplus.bin', myplus);
save(libdir + '/yourplus.bin', yourplus);
save(libdir + '/A.bin', A);

//create the name file
mputl(['myplus';'yourplus';'A'], TMPDIR + '/names');

//build the library containing myplus and yourplus
xlib = lib(libdir + '/');
if load_ref_nocheck('xlib') then   pause,end,


//erase the variables
%ans = clear('myplus', 'yourplus', 'A');
if load_ref('%ans') then   pause,end,


//Automatic loading and execution
%ans = myplus(1, 2);
if load_ref('%ans') then   pause,end,


A;
if load_ref('A') then   pause,end,

xdel_run(winsid());

mclose(%U);
