// Demo file for ext5c example 

// builder code for ext5c.c 
link_name = 'ext5c';    // functions to be added to the call table 
flag  = "c";		// ext5c is a C function 
files = ['ext5c.o' ];   // objects files for ext5c 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext5c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
// reading vector a in scilab internal stack

Amatrix=[1,2,3];b=[2,3,4];
c=call('ext5c',b,1,'d','out',[1,3],2,'d');
if norm(c-(Amatrix+2*b)) > %eps then pause,end

