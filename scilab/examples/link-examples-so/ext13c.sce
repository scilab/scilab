// Demo file for ext13c example 

// builder code for ext13c.c 
// functions to be added to the call table 
link_name = ['ext13ic','ext13oc']; 
flag  = "c";		 // ext13c is a C function 
files = ['ext13c.o' ];   // objects files for ext13c 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext13c and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//sharing common data

a=1:10;
n=10;a=1:10;
call('ext13ic',n,1,'i',a,2,'r','out',2);  //loads b with a
c=call('ext13oc',n,1,'i','out',[1,10],2,'r');  //loads c with b
if norm(c-a) > %eps then pause,end


