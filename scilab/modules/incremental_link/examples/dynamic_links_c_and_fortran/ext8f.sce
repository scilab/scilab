// Demo file for ext8f example 

// builder code for ext8f.c 
link_name = 'ext8f';    // functions to be added to the call table 
flag  = "f";		 // ext8f is a C function 
files = ['ext8f.o' ];   // objects files for ext8f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext8f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the ode function 
//same example as # 10 with call to matptr
//param must be defined as a scilab variable

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext8f');


