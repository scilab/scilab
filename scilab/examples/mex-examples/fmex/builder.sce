// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libmex' 		// interface library name 

// objects files (but do not give mexfiles here)

files = [];

// other libs needed for linking (must be shared library names)

libs  = [] 				

// table of (scilab_name,interface-name or mexfile-name, type) 

table =['mexf1','mexfunction1','fmex';
	'mexf2','mexfunction2','fmex';
	'mexf3','mexfunction3','fmex'];

ldflags = "";
cflags = "";
fflags = "";

// do not modify below 
// ----------------------------------------------
ilib_mex_build(ilib_name,table,files,libs,'Makelib',ldflags,cflags,fflags)
