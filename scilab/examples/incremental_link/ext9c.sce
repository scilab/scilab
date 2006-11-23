// Demo file for ext8c example 

// builder code for ext9c.c 
link_name = 'ext9c';    // functions to be added to the call table 
flag  = "c";		// ext9c is a C function 
files = ['ext9c.o' ];   // objects files for ext9c 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext9c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

//test new function through the call function 
//argument function for ode
//call ext9c argument function with dynamic link

ode([1;0;0],0,[0.4,4],'ext9c');
