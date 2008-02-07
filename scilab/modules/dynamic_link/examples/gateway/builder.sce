// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libtutorial'; 		// interface library name 
files = ['sci_view.o','displ.o','sci_matmul.o','matmul.o'];     // objects files 
					// 
libs  = []; 				// other libs needed for linking
table = [ 'matmul','sci_matmul';
          'view', 'sci_view'];		// table of (scilab_name,interface-name) 

// do not modify below 
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs);



