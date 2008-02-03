// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libtutorial' 		// interface library name 
files = ['sci_view.o','sci_matmul.o','displ.o','matmul.o']     // objects files 
					// 
libs  = []; 				// other libs needed for linking
table = [ 'view', 'sci_view';		// table of (scilab_name,interface-name) 
          'matmul','sci_matmul'];	// for fortran coded interface use 'C2F(name)'

// do not modify below 
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs)



