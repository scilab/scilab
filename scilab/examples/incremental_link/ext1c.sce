// Demo file for ext1c example 

// builder code for ext1c.c 
link_name = 'ext1c';    // functions to be added to the call table 
flag  = "c";		// ext1c is a C function 
files = ['ext1c.o' ];   // objects files for ext1c 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext1c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//(very) simple example 1

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1c',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end






