// Demo file for ext1f example 

// builder code for ext1f.c 
link_name = 'ext1f';    // functions to be added to the call table 
flag  = "f";		// ext1f is a C function 
files = ['ext1f.o' ];   // objects files for ext1f 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext1f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//(very) simple example 1

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext1f',n,1,'i',a,2,'d',b,3,'d','out',[1,3],4,'d');
if norm(c-(a+b)) > %eps then pause,end

