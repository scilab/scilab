// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libmex' 		// interface library name 

// objects files (but do not give mexfiles here)

files = [];

// other libs needed for linking (must be shared library names)

libs  = [] 				

// table of (scilab_name,interface-name or mexfile-name, type) 

table =['mexf01','mexfunction01','cmex';
	'mexf02','mexfunction02','cmex';
	'mexf03','mexfunction03','cmex';
	'mexf04','mexfunction04','cmex';
	'mexf05','mexfunction05','cmex';
	'mexf06','mexfunction06','cmex';
	'mexf07','mexfunction07','cmex';
	'mexf08','mexfunction08','cmex';
	'mexf09','mexfunction09','cmex';
	'mexf10','mexfunction10','cmex';
	'mexf11','mexfunction11','cmex';
	'mexf12','mexfunction12','cmex';
	'mexf13','mexfunction13','cmex';
	'mexf14','mexfunction14','cmex';
	'mexf15','mexfunction15','cmex';];
//Uncomment next 2 lines for a Fortran example
//table = [table;
//   	'mexf16','mexfunction16','fmex'];

ldflags = "";
cflags = "";
fflags = "";

// do not modify below 
// ----------------------------------------------
ilib_mex_build(ilib_name,table,files,libs,'Makelib',ldflags,cflags,fflags)
