// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libmex' 		// interface library name 

// objects files (but do not give mexfiles here)

files = [];

// other libs needed for linking (must be shared library names)

libs  = [] 

// table of (scilab_name,interface-name or mexfile-name, type) 

table =['mexf15','mexfunction15','cmex';];
//Uncomment next 2 lines for a Fortran example
//table = [table;
//   	'mexf16','mexfunction16','fmex'];

ldflags = "";
cflags = "";
fflags = "";

// do not modify below 
// ----------------------------------------------
ilib_mex_build(ilib_name,table,files,libs,'Makelib',ldflags,cflags,fflags)
