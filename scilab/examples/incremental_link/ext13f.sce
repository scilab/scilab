// Demo file for ext13f example 

// builder code for ext13f.c 
// functions to be added to the call table 
link_name = ['ext13if','ext13of']; 
flag  = "f";		 // ext13f is a C function 
files = ['ext13f.o' ];   // objects files for ext13f 
libs  = [];		 // other libs needed for linking 

// the next call generates files (Makelib,loader.sce) used
// for compiling and loading ext13f and performs the compilation

ilib_for_link(link_name,files,libs,flag);

// load new function code in the scope of call 
// using the previously generated loader 
exec loader.sce; 

// test new function through the call function 
//sharing common data

a=1:10;
n=10;a=1:10;
call('ext13if',n,1,'i',a,2,'r','out',2);  //loads b with a
c=call('ext13of',n,1,'i','out',[1,10],2,'r');  //loads c with b
if norm(c-a) > %eps then pause,end




