// This is the builder.sce 
// must be run from this directory 

pathname = get_absolute_file_path('builder.sce') 

disp(pathname)

ilib_name  = 'libfsum' 		// interface library name 
 
// objects files 

files = [pathname+'sci_gateway/sci_fsum.o',pathname+'src/fortran/fsum.o' ];

libs  = [] 				// other libs needed for linking

table =['fortransum',	'sci_fsum'];

// do not modify below 
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs)

