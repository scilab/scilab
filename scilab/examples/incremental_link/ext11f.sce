// Demo file for ext11f example 

// builder code for ext11f.c 
link_name = 'ext11f';    // functions to be added to the call table 
flag  = "f";		 // ext11f is a C function 
files = ['ext11f.o' ];   // objects files for ext11f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext11f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the ode function 
// Passing a parameter to argument funtion of ode

param=[0.04,10000,3d+7];
y=ode([1;0;0],0,[0.4,4],'ext11f')
//param must be defined as a scilab variable upon calling ode



