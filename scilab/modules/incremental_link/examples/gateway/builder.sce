// This is the builder.sce 
// must be run from this directory 

ilib_name  = 'libtutorial' 		// interface library name 
files = ['intview.o','intmatmul.o']     // objects files 
					// 
libs  = [] 				// other libs needed for linking
table = [ 'view', 'intview';		// table of (scilab_name,interface-name) 
          'matmul','intmatmul'];	// for fortran coded interface use 'C2F(name)'

// do not modify below 
// ----------------------------------------------
ilib_build(ilib_name,table,files,libs)



