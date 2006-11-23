// Demo file for ext9f example 

// builder code for ext9f.c 
link_name = 'ext9f';    // functions to be added to the call table 
flag  = "f";  		// ext9f is a C function 
files = ['ext9f.o' ];   // objects files for ext9f 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext9f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce;  

//test new function through the call function 
//argument function for ode
//call ext9f argument function with dynamic link

ode([1;0;0],0,[0.4,4],'ext9f');
