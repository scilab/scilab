// Demo file for ext8f example 

// builder code for ext8f.c 
link_name = 'ext8f';    // functions to be added to the call table 
flag  = "f";		// ext8f is a C function 
files = ['ext8f.o' ];   // objects files for ext8f 
libs  = [];		// other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext8f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//same example as # 10 with call to matptr
//param must be defined as a scilab variable

param=[1,2,3];
y=call('ext8f','out',size(param),1,'d');
if norm(y-param) > %eps then pause,end
if norm(param(1)-18.0) > %eps then pause,end


