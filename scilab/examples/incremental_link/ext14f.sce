// Demo file for ext14f example 

// builder code for ext14f.c 
// functions to be added to the call table 
link_name = ['ext14fi']; 
flag  = "f";		 // ext14f is a C function 
files = ['ext14f.o' ];   // objects files for ext14f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext14f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//(very) simple example 1

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext14fi',a,b);
if norm(c-(a+b)) > %eps then pause,end


