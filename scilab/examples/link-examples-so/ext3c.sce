// Demo file for ext3c example 

// builder code for ext3c.c 
link_name = 'ext3c';    // functions to be added to the call table 
flag  = "c";		// ext3c is a C function 
files = ['ext3c.o' ];   // objects files for ext3c 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext3c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 

a=[1,2,3];b=[4,5,6];n=3;
c=call('ext3c','yes',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d');
if norm(c-(sin(a)+cos(b)))> %eps then pause,end
c=call('ext3c','no',1,'c',n,2,'i',a,3,'d',b,4,'d','out',[1,3],5,'d');
if norm(c-(a+b)) > %eps then pause,end
