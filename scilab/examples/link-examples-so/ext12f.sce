// Demo file for ext12f example 

// builder code for ext12f.c 
link_name = 'ext12f';    // functions to be added to the call table 
flag  = "f";		 // ext12f is a C function 
files = ['ext12f.o' ];   // objects files for ext12f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext12f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the ode function 
//same example as # 10 with call to matptr
//param must be defined as a scilab variable

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext12f');


