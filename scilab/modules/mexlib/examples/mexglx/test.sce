// Linking a .mexglx, say foo.mexglx, file with Scilab.
// (Assuming foo.mexglx has been created by the Matlab's mex script).
// 0/ Here I create the file foo.mexglx using Scilab, just to have a 
// proper foo.mexglx for testing 

ilib_for_link("foo",['foo.o'],[],"c");
host('cp libfoo.so foo.mexglx');

// 1/ If necessary, create empty libmx.so libmex.so and libmat.so which 
//    could be required by the .mexglx file.
//    (If "ldd foo.mexglx" shows a dependency). 
// This is done by the following commands:

ilib_for_link("mx",[],[],"c");
ilib_for_link("mex",[],[],"c");
ilib_for_link("mat",[],[],"c");

// 2/link the (almost empty) .so files with Scilab
// Note that this is not really  usefull 

//link ./libmx.so;
//link ./libmex.so;
//link ./libmat.so;

// 3/link foo.mexglx with Scilab
link ./foo.mexglx;

// 4/ Make a dynamic library with the provided C routine (libtst.c file).
//    Note that you can use libtst.c file as is and that the entrypoint 
//    MUST BE mexFunction. If you have more than one mexglx files you 
//    will need to copy libtst.c and change only the function name 
//    (this is described below) 

ilib_for_link("tst",['libtst.o'],[],"c");

//5/At Scilab prompt enter:

addinter('./libtst.so','libtst','foo');

// 6/call the mexfunction:

foo(5,'test string')

