// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libexamples' 		// interface library name 


// objects files 

files = ['ex18intc.o'];


libs  = [] 				// other libs needed for linking

// table of (scilab_name,interface-name) 
// for fortran coded interface use 'C2F(name)'

table =['ex18c',	'intex18c'];


// extra parameters can be transmited to the linker 
// and to the C and Fortran compilers with 
// ldflags,cflags,fflags 
// for example to link a set of routines using the 
// ImageMagick library 
//  ldflags = "`Magick-config --ldflags --libs`"; 
//  cflags  = "`Magick-config --cppflags`"; 
//  fflags   = ""; 

ldflags = "";
cflags ="-g";
fflags ="";

// do not modify below 
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs,'Makelib',ldflags,cflags,fflags)










