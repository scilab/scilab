// Demo file for ext10f example 

// builder code for ext10f.c 
link_name = 'ext10f';    // functions to be added to the call table 
flag  = "f";		 // ext10f is a C function 
files = ['ext10f.o' ];   // objects files for ext10f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext10f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the ode function 
//passing a parameter to ext10f routine by a list:

param=[0.04,10000,3d+7];    
y=ode([1;0;0],0,[0.4,4],list('ext10f',param));



